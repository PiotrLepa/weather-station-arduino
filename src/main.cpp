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
  // i++;
  // Serial.println(i);
  // WeatherModel model = WeatherModel(i);
  // weatherRepository.sendWeatherData(model);

  if (pmController.read()) {
    PmModel pmModel = pmController.getData();
    Serial.print(F("PM1.0 "));
    Serial.print(pmModel.pm1);
    Serial.print(F(", "));
    Serial.print(F("PM2.5 "));
    Serial.print(pmModel.pm25);
    Serial.print(F(", "));
    Serial.print(F("PM10 "));
    Serial.print(pmModel.pm10);
    Serial.println(F(" [ug/m3]"));
  } else {
    Serial.println(pmController.getErrorMessage());
  }

  delay(10000);
}