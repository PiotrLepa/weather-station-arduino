#include "weather_repository.h"

WeatherRepository::WeatherRepository(RestClient& _client, JsonCoder& _jsonCoder, SdCardStorage& _sdCardStorage)
    : client(_client), jsonCoder(_jsonCoder), sdCardStorage(_sdCardStorage) {}

bool WeatherRepository::sendWeatherData(WeatherModel weather) {
  String json = jsonCoder.encodeWeatherModel(weather);
  int resultCode = client.post("/weather", json);
  if (resultCode == 201) {
    sendCachedWeathers();
    return true;
  } else {
    cacheWeather(weather);
    return false;
  }
}

bool WeatherRepository::sendRainDetected() {
  int resultCode = client.post("/weather/rain-detected");
  if (resultCode == 200) {
    return true;
  } else {
    return false;
  }
}

void WeatherRepository::sendCachedWeathers() {
  std::vector<String> jsonModels = sdCardStorage.readAllInDirectory(CACHED_WEATHERS_PATH);
  if (jsonModels.size() == 0) return;

  String resultJson = jsonCoder.encodeCachedWeathersList(jsonModels);
  int resultCode = client.post("/weather/cached", resultJson);
  if (resultCode == 201) {
    sdCardStorage.removeAllInDirectory(CACHED_WEATHERS_PATH);
  }
}

void WeatherRepository::cacheWeather(WeatherModel weather) {
  DateTime dateTime = DateTime::now();
  if (dateTime.getSecondsFromEpoch() == -1) return;

  CachedWeatherModel cachedWeatherModel = CachedWeatherModel(weather, dateTime.getFormattedDate());
  String fileName = CACHED_WEATHERS_PATH + "/" + String(dateTime.getSecondsFromEpoch()) + TXT_EXT;
  String json = jsonCoder.encodeCachedWeatherModel(cachedWeatherModel);
  sdCardStorage.write(fileName, json);
}