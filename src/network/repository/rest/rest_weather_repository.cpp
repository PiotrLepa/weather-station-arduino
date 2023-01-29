#include "rest_weather_repository.h"

RestWeatherRepository::RestWeatherRepository(RestClient& _client, JsonCoder& _jsonCoder,
                                             SdCardStorage& _sdCardStorage)
    : client(_client), jsonCoder(_jsonCoder), sdCardStorage(_sdCardStorage) {}

bool RestWeatherRepository::sendWeatherData(WeatherModel weather) {
  String json = jsonCoder.encodeWeather(weather);
  int resultCode = client.post("/weather", json);
  if (resultCode == HTTP_CODE_CREATED) {
    sendCachedWeathers();
    return true;
  } else {
    LOGGER.log("Sending weather data to rest server failed with code: " + resultCode);
    cacheWeather(weather);
    return false;
  }
}

bool RestWeatherRepository::sendRainDetected() {
  delay(2000);
  int resultCode = client.post("/weather/rain-detected");
  Serial.println("Rain detected");
  if (resultCode == HTTP_CODE_OK) {
    return true;
  } else {
    LOGGER.log("Sending rain detection to rest server failed with code: " + resultCode);
    return false;
  }
}

void RestWeatherRepository::sendCachedWeathers() {
  std::vector<String> jsonModels = sdCardStorage.readAllInDirectory(CACHED_WEATHERS_PATH);
  if (jsonModels.size() == 0) return;

  String resultJson = jsonCoder.encodeWeathersList(jsonModels);
  int resultCode = client.post("/weather/cached", resultJson);
  if (resultCode == HTTP_CODE_CREATED) {
    sdCardStorage.removeAllInDirectory(CACHED_WEATHERS_PATH);
  } else {
    LOGGER.log("Sending cached weathers to rest server failed with code: " + resultCode);
  }
}

void RestWeatherRepository::cacheWeather(WeatherModel weather) {
  String fileName = getFileNameToCacheWeather(weather.timestamp);
  String json = jsonCoder.encodeWeather(weather);
  bool weatherSaved = sdCardStorage.write(fileName, json);
  if (!weatherSaved) {
    LOGGER.log("Save weather to sd card failed");
  }
}

String RestWeatherRepository::getFileNameToCacheWeather(String formattedDate) {
  String timestampWithoutInvalidChars = String(formattedDate);
  timestampWithoutInvalidChars.replace(':', '-');
  timestampWithoutInvalidChars.replace(".000", "");
  return CACHED_WEATHERS_PATH + "/" + timestampWithoutInvalidChars + TXT_EXT;
}