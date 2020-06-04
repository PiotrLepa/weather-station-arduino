#include "main.h"

RestClient restClient = RestClient(API_URL);
JsonEncoder jsonEncoder = JsonEncoder();
WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  restClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);
}

int i = 0;
void loop() {
  i++;
  Serial.println(i);
  WeatherModel model = WeatherModel(i);
  weatherRepository.sendWeatherData(model);
  delay(5000);
}