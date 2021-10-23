#include "main.h"

WifiClient wifiClient = WifiClient();
RestClient restClient = RestClient(API_URL);
JsonCoder jsonCoder = JsonCoder();

SdCardStorage sdCardStorage = SdCardStorage();

WeatherRepository weatherRepository = WeatherRepository(restClient, jsonCoder, sdCardStorage);

TemperatureReader tempReader = TemperatureReader(TEMPERATURE_SENSOR_PIN);
OneWire externalTemperatureOneWire(EXTERNAL_TEMPERATURE_SENSOR_PIN);
ExternalTemperatureReader externalTempReader = ExternalTemperatureReader(&externalTemperatureOneWire);
PressureReader pressureReader = PressureReader();
AirQualityReader airQualityReader = AirQualityReader(Serial, PMS_MODE_CONTROL_PIN);
WindReader windReader = WindReader(WIND_SENSOR_PIN);
RainGaugeReader rainGaugeReader = RainGaugeReader(RAIN_GAUGE_SENSOR_PIN);

Ticker wakeUpSensorsTimer = Ticker(wakeUpSensors, SERVER_REQUEST_DELAY);
Ticker collectWeatherDataTimer = Ticker(collectWeatherData, PMS_WAKE_UP_MILLIS);

bool sendRainDetectedRequest = false;

class MyRainGaugeCallbacks : public RainGaugeCallbacks {
  void rainDetected() {
    // Do not call weatherRepository.sendRainDetected() here. HttpClient will return an error.
    sendRainDetectedRequest = true;
  }
};

void setup() {
  Serial.begin(9600);

  pinMode(WIFI_STATUS_PIN, OUTPUT);

  wifiClient.begin();
  sdCardStorage.begin();
  tempReader.begin();
  externalTempReader.begin();
  pressureReader.begin();
  airQualityReader.begin();
  windReader.begin();
  rainGaugeReader.begin();
  rainGaugeReader.setCallback(new MyRainGaugeCallbacks());

  connectToWifi();

  startSensors();
}

void loop() {
  wakeUpSensorsTimer.update();
  collectWeatherDataTimer.update();
  windReader.update();
  checkIfRainHasBeenDetected();
}

void startSensors() {
  windReader.startReading();
  rainGaugeReader.startReading();
}

ConnectionResult connectToWifi() {
  /*
  To configure WIFI_SSID and WIFI_PASSWORD create file private_config.h in /src/config folder

  Example to copy:

  #ifndef PRIVATE_CONFIG_H
  #define PRIVATE_CONFIG_H

  #define WIFI_SSID "WIFI_SSID"
  #define WIFI_PASSWORD "WIFI_PASSWORD"

  #endif

  */

  ConnectionResult result = wifiClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD, 50);
  if (result == CONNECTED) {
    setWifiLed(true);
    wakeUpSensorsTimer.start();
    DateTime::begin();
  } else {
    setWifiLed(false);
  }
  return result;
}

void setWifiLed(bool isWifiEnabled) { digitalWrite(WIFI_STATUS_PIN, isWifiEnabled); }

void checkIfRainHasBeenDetected() {
  if (sendRainDetectedRequest) {
    sendRainDetectedRequest = false;
    weatherRepository.sendRainDetected();
  }
}

void wakeUpSensors() {
  airQualityReader.wakeUp();
  wakeUpSensorsTimer.stop();
  collectWeatherDataTimer.start();
}

void collectWeatherData() {
  collectWeatherDataTimer.stop();

  windReader.stopReading();
  rainGaugeReader.stopReading();

  TemperatureModel temperatureModel;
  if (tempReader.read()) {
    temperatureModel = tempReader.getData();
  } else {
    Serial.println(tempReader.getErrorMessage());
  }

  ExternalTemperatureModel externalTemperatureModel;
  if (externalTempReader.read()) {
    externalTemperatureModel = externalTempReader.getData();
  } else {
    Serial.println(externalTempReader.getErrorMessage());
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

  WindModel windModel = windReader.getData();
  RainGaugeModel rainGaugeModel = rainGaugeReader.getData();

  sendWeatherDataToServer(temperatureModel, externalTemperatureModel, pressureModel, airQualityModel, windModel,
                          rainGaugeModel);

  airQualityReader.sleep();
  startSensors();
  wakeUpSensorsTimer.start();
}

void sendWeatherDataToServer(TemperatureModel temperature, ExternalTemperatureModel externalTemperature,
                             PressureModel pressureModel, AirQualityModel airQuality, WindModel wind,
                             RainGaugeModel rainGauge) {
  WeatherModel model = WeatherModel(temperature, externalTemperature, pressureModel, airQuality, wind, rainGauge);
  if (model.canBeSendToServer()) {
    weatherRepository.sendWeatherData(model);
  } else {
    Serial.println("Weather model is incorrect");
  }
}