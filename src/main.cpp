#include "main.h"

RestClient restClient = RestClient(API_URL);
JsonEncoder jsonEncoder = JsonEncoder();
WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);
PmController pmController = PmController(Serial);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  restClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);
  pmController.begin();
}

int i = 0;
void loop() {
  i++;
  Serial.println(i);
  TemperatureModel temperatureModel = TemperatureModel(i);

  if (pmController.read()) {
    AirQualityModel airQualityModel = pmController.getData();
    Serial.print(F("\nPM1.0 "));
    Serial.print(airQualityModel.pm1);
    Serial.print(F(", "));
    Serial.print(F("PM2.5 "));
    Serial.print(airQualityModel.pm25);
    Serial.print(F(", "));
    Serial.print(F("PM10 "));
    Serial.print(airQualityModel.pm10);
    Serial.println(F(" [ug/m3]"));
  } else {
    Serial.println(pmController.getErrorMessage());
  }

  WeatherModel model = WeatherModel(temperatureModel, pmController.getData());
  weatherRepository.sendWeatherData(model);

  delay(10000);
}