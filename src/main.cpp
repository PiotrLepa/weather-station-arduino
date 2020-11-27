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
AirQualityReader airQualityReader = AirQualityReader(Serial);
WindReader windReader = WindReader(WIND_SENSOR_PIN);
RainGaugeReader rainGaugeReader = RainGaugeReader(RAIN_GAUGE_SENSOR_PIN);
LocationReader locationReader = LocationReader(GPS_SENSOR_TX_PIN, GPS_SENSOR_RX_PIN);

Ticker serverRequestTimer = Ticker(gatherWeatherData, SERVER_REQUEST_DELAY);
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
  windReader.begin();
  rainGaugeReader.begin();
  rainGaugeReader.setCallback(new MyRainGaugeCallbacks());
  bleManager.begin(new MyBleCallbacks());
  connectToWifiIfCredentialsAreSaved();

  // Have to be called last
  locationReader.begin();

  startSensors();
}

void loop() {
  serverRequestTimer.update();
  windReader.update();
  startScanWifiTimer.update();
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
    connectToWifiAndSetupOnSuccess(credentialsJson, false);
  }
}

ConnectionResult connectToWifiAndSetupOnSuccess(String credentialsJson, bool saveCredentials) {
  WifiCredentialsModel credentials = jsonCoder.decodeWifiCredentials(credentialsJson);
  ConnectionResult result = wifiClient.connectToWifi(credentials.name, credentials.password);
  if (result == CONNECTED) {
    serverRequestTimer.start();
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

void gatherWeatherData() {
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

  startSensors();
}

void sendWeatherDataToServer(TemperatureModel temperature, PressureModel pressureModel, AirQualityModel airQuality,
                             WindModel wind, RainGaugeModel rainGauge, LocationModel location) {
  WeatherModel model = WeatherModel(temperature, pressureModel, airQuality, wind, rainGauge, location);
  weatherRepository.sendWeatherData(model);
}