#include "main.h"

WifiClient wifiClient = WifiClient();
// RestClient restClient = RestClient(API_URL);
FirestoreClient firestoreClient = FirestoreClient(FIREBASE_PROJECT_ID);
JsonCoder jsonCoder = JsonCoder();

SdCardStorage sdCardStorage = SdCardStorage();

WeatherModelRounder weatherModelRounder = WeatherModelRounder();
WeatherPrinter weatherPrinter = WeatherPrinter();

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

  externalTempReader.read();
  ExternalTemperatureModel externalTemperatureModel = externalTempReader.getData();

  pressureReader.read();
  PressureModel pressureModel = pressureReader.getData();

  airQualityReader.read();
  AirQualityModel airQualityModel = airQualityReader.getData();

  WindModel windModel = windReader.getData();
  RainGaugeModel rainGaugeModel = rainGaugeReader.getData();

  String errorMessage =
      externalTempReader.getErrorMessage() + "\n" + pressureReader.getErrorMessage() + "\n" + airQualityReader.getErrorMessage() + "\n";

  if (errorMessage != "") {
    errorMessage = "Weather model is incorrect\n" + errorMessage;
    LOGGER.log(errorMessage);
  }

  sendWeatherDataToServer(externalTemperatureModel, pressureModel, airQualityModel, windModel, rainGaugeModel);

  airQualityReader.sleep();
  startSensors();
  wakeUpSensorsTimer.start();
}

void sendWeatherDataToServer(ExternalTemperatureModel externalTemperature, PressureModel pressureModel, AirQualityModel airQuality,
                             WindModel wind, RainGaugeModel rainGauge) {
  WeatherModel weather = WeatherModel(externalTemperature, pressureModel, airQuality, wind, rainGauge, getCurrentTimestamp());
  WeatherModel roundedWeather = weatherModelRounder.round(weather);
  weatherPrinter.print(roundedWeather);
  if (roundedWeather.canBeSendToServer()) {
    weatherRepository->sendWeatherData(roundedWeather);
  }
}

String getCurrentTimestamp() {
  DateTime dateTime = DateTime::now();
  if (dateTime.getSecondsFromEpoch() == -1) return "";

  return dateTime.getFormattedDate();
}