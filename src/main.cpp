#include "main.h"

WifiClient wifiClient = WifiClient();
// RestClient restClient = RestClient(API_URL);
FirestoreClient firestoreClient = FirestoreClient(FIREBASE_PROJECT_ID);
JsonCoder jsonCoder = JsonCoder();

SdCardStorage sdCardStorage = SdCardStorage();

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

class MyRainGaugeCallbacks : public RainGaugeCallbacks
{
  void rainDetected() { weatherRepository->sendRainDetected(); }
};

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);

  pinMode(WIFI_STATUS_PIN, OUTPUT);

  wifiClient.begin();
  sdCardStorage.begin();
  externalTempReader.begin();
  pressureReader.begin();
  airQualityReader.begin();
  windReader.begin();
  rainGaugeReader.begin();
  rainGaugeReader.setCallback(new MyRainGaugeCallbacks());

  connectToWifi();

  startSensors();
}

void loop()
{
  wakeUpSensorsTimer.update();
  collectWeatherDataTimer.update();
  windReader.update();
  rainGaugeReader.update();
}

void startSensors()
{
  windReader.startReading();
  rainGaugeReader.startReading();
}

ConnectionResult connectToWifi()
{
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
  if (result == CONNECTED)
  {
    setWifiLed(true);
    wakeUpSensorsTimer.start();
    DateTime::begin();
  }
  else
  {
    setWifiLed(false);
  }
  return result;
}

void setWifiLed(bool isWifiEnabled) { digitalWrite(WIFI_STATUS_PIN, isWifiEnabled); }

void wakeUpSensors()
{
  airQualityReader.wakeUp();
  wakeUpSensorsTimer.stop();
  collectWeatherDataTimer.start();
}

void collectWeatherData()
{
  collectWeatherDataTimer.stop();

  windReader.stopReading();
  rainGaugeReader.stopReading();

  ExternalTemperatureModel externalTemperatureModel;
  if (externalTempReader.read())
  {
    externalTemperatureModel = externalTempReader.getData();
  }
  else
  {
    Serial.println(externalTempReader.getErrorMessage());
  }

  PressureModel pressureModel;
  if (pressureReader.read())
  {
    pressureModel = pressureReader.getData();
  }
  else
  {
    Serial.println(pressureReader.getErrorMessage());
    // pressureReader.begin(); // TODO remove?
  }

  AirQualityModel airQualityModel;
  if (airQualityReader.read())
  {
    airQualityModel = airQualityReader.getData();
  }
  else
  {
    Serial.println(airQualityReader.getErrorMessage());
  }

  WindModel windModel = windReader.getData();
  RainGaugeModel rainGaugeModel = rainGaugeReader.getData();

  sendWeatherDataToServer(externalTemperatureModel, pressureModel, airQualityModel, windModel, rainGaugeModel);

  airQualityReader.sleep();
  startSensors();
  wakeUpSensorsTimer.start();
}

void sendWeatherDataToServer(ExternalTemperatureModel externalTemperature, PressureModel pressureModel,
                             AirQualityModel airQuality, WindModel wind, RainGaugeModel rainGauge)
{
  WeatherModel model = WeatherModel(externalTemperature, pressureModel, airQuality, wind, rainGauge);
  if (model.canBeSendToServer())
  {
    weatherRepository->sendWeatherData(model);
  }
  else
  {
    Serial.println("Weather model is incorrect");
    Serial.println();
  }
}