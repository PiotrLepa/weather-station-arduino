#include "main.h"

RestClient restClient = RestClient(API_URL, WIFI_SSID, WIFI_PASSWORD);

JsonEncoder jsonEncoder = JsonEncoder();

WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);

void setup() { Serial.begin(9600); }

int i = 0;
void loop() {
  i++;
  WeatherModel model = WeatherModel(i);
  weatherRepository.sendWeatherData(model);
  delay(5000);
}