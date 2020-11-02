#include "weather_repository.h"

WeatherRepository::WeatherRepository(RestClient& _client, JsonCoder& _jsonCoder,
                                     SdCardManager& _sdCardManager,
                                     DateTime& _dateTime)
    : client(_client),
      jsonCoder(_jsonCoder),
      sdCardManager(_sdCardManager),
      dateTime(_dateTime) {}

bool WeatherRepository::sendWeatherData(WeatherModel weather) {
  String json = jsonCoder.encodeWeatherModel(weather);
  int resultCode = client.post("/weather/current", json);
  if (resultCode == 201) {
    return true;
  }

  CachedWeatherModel cachedWeatherModel =
      CachedWeatherModel(weather, dateTime.now());
  String fileName = "/unsent_weather_data/" + String(millis()) + ".txt";
  sdCardManager.write(fileName, json);

  return false;
}