#include "weather_repository.h"

WeatherRepository::WeatherRepository(RestClient& _client, JsonCoder& _jsonCoder,
                                     SdCardManager& _sdCardManager)
    : client(_client), jsonCoder(_jsonCoder), sdCardManager(_sdCardManager) {}

bool WeatherRepository::sendWeatherData(WeatherModel model) {
  String json = jsonCoder.encodeWeatherModel(model);
  int resultCode = client.post("/weather/current", json);
  if (resultCode == 201) {
    return true;
  }

  String fileName = "/unsent_weather_data/" + String(millis()) + ".txt";
  // String fileName = String(millis()) + ".json";
  sdCardManager.write(fileName, json);

  return false;
}