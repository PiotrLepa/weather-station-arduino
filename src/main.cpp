#include "main.h"

WifiClient wifiClient = WifiClient();
// RestClient restClient = RestClient(API_URL);
FirestoreClient firestoreClient = FirestoreClient(FIREBASE_PROJECT_ID);
JsonCoder jsonCoder = JsonCoder();

SdCardStorage sdCardStorage = SdCardStorage();

WeatherModelRounder weatherModelRounder = WeatherModelRounder();
WeatherModelFormatter weatherModelFormatter = WeatherModelFormatter();

// WeatherRepository *weatherRepository = new RestWeatherRepository(restClient, jsonCoder, sdCardStorage);
WeatherRepository *weatherRepository = new FirestoreWeatherRepository(firestoreClient, jsonCoder, sdCardStorage);

OneWire externalTemperatureOneWire(EXTERNAL_TEMPERATURE_SENSOR_PIN);
ExternalTemperatureReader externalTempReader = ExternalTemperatureReader(&externalTemperatureOneWire);
PressureReader pressureReader = PressureReader();
AirQualityReader airQualityReader = AirQualityReader(Serial2, PMS_MODE_CONTROL_PIN);
WindReader windReader = WindReader(WIND_SENSOR_PIN);
RainGaugeReader rainGaugeReader = RainGaugeReader(RAIN_GAUGE_SENSOR_PIN);

Ticker wakeUpSensorsTimer = Ticker(wakeUpSensors, SERVER_REQUEST_DELAY);
Ticker collectWeatherDataTimer = Ticker(collectWeatherData, PMS_WAKE_UP_MILLIS);

class MyRainGaugeCallbacks : public RainGaugeCallbacks {
  void rainDetected() { weatherRepository->sendRainDetected(); }
};

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  LOGGER.begin();
  LOGGER.printSavedLogs();
  // LOGGER.clear();

  pinMode(WIFI_STATUS_PIN, OUTPUT);
  wifiClient.begin();

  connectToWifi();

  String errorMessage = "";
  if (!sdCardStorage.begin()) {
    errorMessage = "Sd card initialization error\n";
  }
  if (!externalTempReader.begin()) {
    errorMessage = "External temperature initialization error\n";
  }
  if (!pressureReader.begin()) {
    errorMessage = "Pressure initialization error\n";
  }
  if (!airQualityReader.begin()) {
    errorMessage = "Air quality initialization error\n";
  }
  if (!windReader.begin()) {
    errorMessage = "Wind reader initialization error\n";
  }
  if (!rainGaugeReader.begin()) {
    errorMessage = "Rain gauge initialization error\n";
  }

  if (errorMessage != "") {
    LOGGER.log(errorMessage);
  }

  rainGaugeReader.setCallback(new MyRainGaugeCallbacks());

  firestoreClient.connect();

  startSensors();
}

void loop() {
  wakeUpSensorsTimer.update();
  collectWeatherDataTimer.update();
  windReader.update();
  rainGaugeReader.update();
}

void startSensors() {
  windReader.startReading();
  rainGaugeReader.startReading();
}

ConnectionResult connectToWifi() {
  /*
  To configure WIFI_SSID and WIFI_PASSWORD create file wifi_credentials.h in /src/config folder

  Example to copy:

  #ifndef WIFI_CREDENTIALS_H
  #define WIFI_CREDENTIALS_H

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

void wakeUpSensors() {
  airQualityReader.wakeUp();
  wakeUpSensorsTimer.stop();
  collectWeatherDataTimer.start();
}

void collectWeatherData() {
  collectWeatherDataTimer.stop();

  windReader.stopReading();
  rainGaugeReader.stopReading();

  ExternalTemperatureModel externalTemperatureModel = externalTempReader.read();
  PressureModel pressureModel = pressureReader.read();
  AirQualityModel airQualityModel = airQualityReader.read();

  WindModel windModel = windReader.getData();
  RainGaugeModel rainGaugeModel = rainGaugeReader.getData();

  sendWeatherDataToServer(externalTemperatureModel, pressureModel, airQualityModel, windModel, rainGaugeModel);

  airQualityReader.sleep();
  startSensors();
  wakeUpSensorsTimer.start();
}

void sendWeatherDataToServer(ExternalTemperatureModel externalTemperature, PressureModel pressureModel, AirQualityModel airQuality,
                             WindModel wind, RainGaugeModel rainGauge) {
  WeatherModel weather = WeatherModel(externalTemperature, pressureModel, airQuality, wind, rainGauge, DateTime::now());
  WeatherModel roundedWeather = weatherModelRounder.round(weather);
  String modelToPrint = weatherModelFormatter.format(roundedWeather);
  if (roundedWeather.canBeSendToServer()) {
    Serial.println(modelToPrint);
    weatherRepository->sendWeatherData(roundedWeather);
  } else {
    if (airQuality.hasError) {
      LOGGER.log(airQualityReader.getErrorMessage());
    }
    LOGGER.log(modelToPrint);
  }
}