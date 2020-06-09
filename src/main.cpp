#include "main.h"

RestClient restClient = RestClient(API_URL);
JsonEncoder jsonEncoder = JsonEncoder();
WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);

PmController pmController = PmController(Serial);
DhtController dhtController = DhtController(DHT_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  restClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);
  dhtController.begin();
  pmController.begin();
}

void loop() {
  if (dhtController.read()) {
    TemperatureModel temperatureModel = dhtController.getData();
    printTemperature(temperatureModel);
  } else {
    Serial.println(dhtController.getErrorMessage());
  }

  if (pmController.read()) {
    AirQualityModel airQualityModel = pmController.getData();
    printAirQuality(airQualityModel);
  } else {
    Serial.println(pmController.getErrorMessage());
  }

  WeatherModel model =
      WeatherModel(dhtController.getData(), pmController.getData());
  weatherRepository.sendWeatherData(model);

  delay(10000);
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