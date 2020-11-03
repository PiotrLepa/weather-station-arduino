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
    sendCachedWeathers();
    return true;
  } else {
    cacheWeather(weather);
    return false;
  }
}

void WeatherRepository::cacheWeather(WeatherModel weather) {
  String timestamp = dateTime.now();
  if (timestamp == "") return;

  CachedWeatherModel cachedWeatherModel = CachedWeatherModel(weather, timestamp);
  String fileName = CACHED_WEATHERS_PATH + "/" + String(millis()) + TXT_EXT;
  String json = jsonCoder.encodeCachedWeatherModel(cachedWeatherModel);
  sdCardManager.write(fileName, json);
}

void WeatherRepository::sendCachedWeathers() {
  std::vector<String> jsonModels =
      sdCardManager.readAllInDirectory(CACHED_WEATHERS_PATH);

  if (jsonModels.size() == 0) return;

  String resultJson = "[";
  for (String json : jsonModels) {
    resultJson += json + ",";
  }
  resultJson.remove(resultJson.length());
  resultJson += "]";

  int resultCode = client.post("/weather/cached", resultJson);
  if (resultCode == 201) {
    sdCardManager.remove(CACHED_WEATHERS_PATH);
  }
}