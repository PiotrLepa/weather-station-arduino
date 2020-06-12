#include "main.h"

RestClient restClient = RestClient(API_URL);
JsonEncoder jsonEncoder = JsonEncoder();
WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);

AirQualityReader airQualityReader = AirQualityReader(Serial);
TemperatureReader tempReader = TemperatureReader(TEMPERATURE_SENSOR_PIN);
WindReader windReader = WindReader(WIND_SENSOR_PIN);

void printWindData();

Ticker windTimer = Ticker(printWindData, 15000);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  windTimer.start();
  // restClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);
  // tempReader.begin();
  // airQualityReader.begin();
  windReader.begin();
  windReader.startReading();
}

void loop() {
  // if (tempReader.read()) {
  //   TemperatureModel temperatureModel = tempReader.getData();
  //   printTemperature(temperatureModel);
  // } else {
  //   Serial.println(tempReader.getErrorMessage());
  // }

  // if (airQualityReader.read()) {
  //   AirQualityModel airQualityModel = airQualityReader.getData();
  //   printAirQuality(airQualityModel);
  // } else {
  //   Serial.println(airQualityReader.getErrorMessage());
  // }

  // WeatherModel model =
  //     WeatherModel(tempReader.getData(), airQualityReader.getData());
  // weatherRepository.sendWeatherData(model);

  // delay(5 * 60 * 1000);
  windTimer.update();
  windReader.update();
}

void printWindData() {
  Serial.println("WIND");

  Serial.println("STOP\n");
  windReader.stopReading();

  WindModel windModel = windReader.getData();
  Serial.println(windModel.windSpeedMax);
  Serial.println(windModel.windSpeedMin);
  Serial.println(windModel.windSpeedAvg);
  Serial.println();

  Serial.println("DELAY\n");
  delay(10000);

  Serial.println("START\n");
  windReader.startReading();
}

void printTemperature(TemperatureModel model) {
  Serial.print(F("\nTemp: "));
  Serial.print(model.temperature);
  Serial.print(F("\nHumidity: "));
  Serial.println(model.humidity);
}

void printAirQuality(AirQualityModel model) {
  Serial.print(F("\nPM1.0: "));
  Serial.print(model.pm1);
  Serial.print(F("\nPM2.5: "));
  Serial.print(model.pm25);
  Serial.print(F("\nPM10: "));
  Serial.println(model.pm10);
}