#include "main.h"

WifiClient wifiClient = WifiClient();
RestClient restClient = RestClient(API_URL);
JsonCoder jsonCoder = JsonCoder();

BleManager bleManager = BleManager(jsonCoder);
SdCardStorage sdCardStorage = SdCardStorage();
EepromStorage eepromStorage = EepromStorage();

WeatherRepository weatherRepository = WeatherRepository(restClient, jsonCoder, sdCardStorage);

TemperatureReader tempReader = TemperatureReader(TEMPERATURE_SENSOR_PIN);
PressureReader pressureReader = PressureReader();
AirQualityReader airQualityReader = AirQualityReader(Serial, PMS_MODE_CONTROL_PIN);
LocationReader locationReader = LocationReader(Serial1, GPS_SENSOR_RX_PIN, GPS_SENSOR_TX_PIN);
WindReader windReader = WindReader(WIND_SENSOR_PIN);
RainGaugeReader rainGaugeReader = RainGaugeReader(RAIN_GAUGE_SENSOR_PIN);

Ticker serverRequestTimer = Ticker(wakeUpSensors, SERVER_REQUEST_DELAY);
Ticker wakeUpSensorsTimer = Ticker(collectWeatherData, PMS_WAKE_UP_MILLIS);
Ticker startScanWifiTimer = Ticker(scanAndSendWifiList, START_SCAN_WIFI_DELAY);

bool sendRainDetectedRequest = false;

class MyBleCallbacks : public BleCallbacks {
  void scanAvailablesWifi() {
    // Give client some time to start observe ble notifications
    startScanWifiTimer.start();
  }

  ConnectionResult connectToWifi(String credentialsJson) {
    return connectToWifiAndSetupOnSuccess(credentialsJson, true);
  }
};

class MyRainGaugeCallbacks : public RainGaugeCallbacks {
  void rainDetected() {
    // Do not call weatherRepository.sendRainDetected() here. HttpClient will return an error.
    sendRainDetectedRequest = true;
  }
};

void scanAndSendWifiList() {
  startScanWifiTimer.stop();
  std::vector<WifiModel> wifiList = wifiClient.scanWifi();
  bleManager.sendWifiList(wifiList);
}

void setup() {
  Serial.begin(9600);
  wifiClient.begin();
  sdCardStorage.begin();
  eepromStorage.begin();
  tempReader.begin();
  pressureReader.begin();
  airQualityReader.begin();
  locationReader.begin();
  windReader.begin();
  rainGaugeReader.begin();
  rainGaugeReader.setCallback(new MyRainGaugeCallbacks());
  bleManager.begin(new MyBleCallbacks());
  connectToWifiIfCredentialsAreSaved();

  startSensors();
}

void loop() {
  serverRequestTimer.update();
  startScanWifiTimer.update();
  wakeUpSensorsTimer.update();
  windReader.update();
  bleManager.update();
  locationReader.update();
  checkIfRainHasBeenDetected();
}

void startSensors() {
  windReader.startReading();
  rainGaugeReader.startReading();
}

void connectToWifiIfCredentialsAreSaved() {
  String credentialsJson = eepromStorage.read(WIFI_CREDENTIALS_ADDRESS);
  if (credentialsJson != NULL) {
    connectToWifiAndSetupOnSuccess(credentialsJson, false, 50);
  }
}

ConnectionResult connectToWifiAndSetupOnSuccess(String credentialsJson, bool saveCredentials, int tries) {
  WifiCredentialsModel credentials = jsonCoder.decodeWifiCredentials(credentialsJson);
  ConnectionResult result = wifiClient.connectToWifi(credentials.name, credentials.password, tries);
  if (result == CONNECTED) {
    wakeUpSensorsTimer.start();
    DateTime::begin();
    if (saveCredentials) {
      eepromStorage.write(credentialsJson, WIFI_CREDENTIALS_ADDRESS);
    }
  }
  return result;
}

void checkIfRainHasBeenDetected() {
  if (sendRainDetectedRequest) {
    sendRainDetectedRequest = false;
    weatherRepository.sendRainDetected();
  }
}

void wakeUpSensors() {
  airQualityReader.wakeUp();

  wakeUpSensorsTimer.start();
}

void collectWeatherData() {
  wakeUpSensorsTimer.stop();

  windReader.stopReading();
  rainGaugeReader.stopReading();

  TemperatureModel temperatureModel;
  if (tempReader.read()) {
    temperatureModel = tempReader.getData();
  } else {
    Serial.println(tempReader.getErrorMessage());
  }

  PressureModel pressureModel;
  if (pressureReader.read()) {
    pressureModel = pressureReader.getData();
  } else {
    Serial.println(pressureReader.getErrorMessage());
  }

  AirQualityModel airQualityModel;
  if (airQualityReader.read()) {
    airQualityModel = airQualityReader.getData();
  } else {
    Serial.println(airQualityReader.getErrorMessage());
  }

  LocationModel locationModel;
  if (locationReader.read()) {
    locationModel = locationReader.getData();
  } else {
    Serial.println(locationReader.getErrorMessage());
  }

  WindModel windModel = windReader.getData();
  RainGaugeModel rainGaugeModel = rainGaugeReader.getData();

  sendWeatherDataToServer(temperatureModel, pressureModel, airQualityModel, windModel, rainGaugeModel, locationModel);

  airQualityReader.sleep();
  startSensors();
}

void sendWeatherDataToServer(TemperatureModel temperature, PressureModel pressureModel, AirQualityModel airQuality,
                             WindModel wind, RainGaugeModel rainGauge, LocationModel location) {
  WeatherModel model = WeatherModel(temperature, pressureModel, airQuality, wind, rainGauge, location);
  if (model.canBeSendToServer()) {
    weatherRepository.sendWeatherData(model);
  } else {
    Serial.println("Weather model is incorrect");
  }
}