#include "main.h"

WifiClient wifiClient = WifiClient();
RestClient restClient = RestClient(API_URL);
JsonCoder jsonCoder = JsonCoder();

BleManager bleManager = BleManager(jsonCoder);

WeatherRepository weatherRepository = WeatherRepository(restClient, jsonCoder);

TemperatureReader tempReader = TemperatureReader(TEMPERATURE_SENSOR_PIN);
PressureReader pressureReader = PressureReader();
AirQualityReader airQualityReader = AirQualityReader(Serial);
WindReader windReader = WindReader(WIND_SENSOR_PIN);
RainGaugeReader rainGaugeReader = RainGaugeReader(RAIN_GAUGE_SENSOR_PIN);
LocationReader locationReader =
    LocationReader(GPS_SENSOR_RX_PIN, GPS_SENSOR_TX_PIN);

Ticker serverRequestTimer = Ticker(gatherWeatherData, SERVER_REQUEST_DELAY);
Ticker scanWifiTimer = Ticker(sendWifiList, SEND_WIFI_LIST_DELAY);

class MyBleCallbacks : public BleCallbacks {
  void scanAvailablesWifi() {
    Serial.println("\nscanAvailablesWifi");
    sendWifiList();
    scanWifiTimer.start();
  }

  void connectToWifi() {
    Serial.println("connectToWifi");
    scanWifiTimer.stop();
  }
};

void sendWifiList() {
  std::vector<WifiModel> wifiList = wifiClient.scanWifi();
  bleManager.sendAvailableWifiList(wifiList);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  wifiClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);

  serverRequestTimer.start();

  begin();
  startSensors();
}

void loop() {
  serverRequestTimer.update();
  windReader.update();
  locationReader.update();
  scanWifiTimer.update();
}

void begin() {
  tempReader.begin();
  pressureReader.begin();
  airQualityReader.begin();
  windReader.begin();
  rainGaugeReader.begin();
  locationReader.begin();
  bleManager.begin(new MyBleCallbacks());
}

void startSensors() {
  windReader.startReading();
  rainGaugeReader.startReading();
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

  sendWeatherDataToServer(temperatureModel, pressureModel, airQualityModel,
                          windModel, rainGaugeModel);

  startSensors();
}

void sendWeatherDataToServer(TemperatureModel temperature,
                             PressureModel pressureModel,
                             AirQualityModel airQuality, WindModel wind,
                             RainGaugeModel rainGauge) {
  WeatherModel model =
      WeatherModel(temperature, pressureModel, airQuality, wind, rainGauge);
  weatherRepository.sendWeatherData(model);
}