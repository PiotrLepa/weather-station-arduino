#include "main.h"

RestClient restClient = RestClient(API_URL);
JsonEncoder jsonEncoder = JsonEncoder();
WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);

AirQualityReader airQualityReader = AirQualityReader(Serial);
TemperatureReader tempReader = TemperatureReader(TEMPERATURE_SENSOR_PIN);
WindReader windReader = WindReader(WIND_SENSOR_PIN);

Ticker windTimer = Ticker(gatherWeatherData, 15000);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  restClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);

  windTimer.start();

  begin();
  startSensors();
}

void loop() {
  windTimer.update();
  windReader.update();
}

void begin() {
  tempReader.begin();
  airQualityReader.begin();
  windReader.begin();
}

void startSensors() { windReader.startReading(); }

void gatherWeatherData() {
  windReader.stopReading();

  TemperatureModel temperatureModel;
  if (tempReader.read()) {
    temperatureModel = tempReader.getData();
  } else {
    Serial.println(tempReader.getErrorMessage());
  }

  AirQualityModel airQualityModel;
  if (airQualityReader.read()) {
    airQualityModel = airQualityReader.getData();
  } else {
    Serial.println(airQualityReader.getErrorMessage());
  }

  WindModel windModel = windReader.getData();

  sendWeatherDataToServer(temperatureModel, airQualityModel, windModel);

  startSensors();
}

void sendWeatherDataToServer(TemperatureModel temperature,
                             AirQualityModel airQuality, WindModel wind) {
  WeatherModel model = WeatherModel(temperature, airQuality, wind);
  weatherRepository.sendWeatherData(model);
}