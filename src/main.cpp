#include "main.h"

RestClient restClient = RestClient(API_URL);
JsonEncoder jsonEncoder = JsonEncoder();
WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);

AirQualityReader airQualityReader = AirQualityReader(Serial);
TemperatureReader tempReader = TemperatureReader(DHT_PIN);
WindReader windReader = WindReader(WIND_SENSOR_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  // restClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);
  // tempReader.begin();
  // airQualityReader.begin();
  windReader.begin();
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

  windReader.startReading(15);
  WindModel model = windReader.getData();
  Serial.println(model.windSpeedMax);
  Serial.println(model.windSpeedMin);
  Serial.println(model.windSpeedAvg);

  delay(15000);
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