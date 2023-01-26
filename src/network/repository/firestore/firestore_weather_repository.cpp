#include "firestore_weather_repository.h"

FirestoreWeatherRepository::FirestoreWeatherRepository(FirestoreClient& _client, WeatherModelRounder& _weatherModelRounder,
                                                       JsonCoder& _jsonCoder, SdCardStorage& _sdCardStorage)
    : client(_client), weatherModelRounder(_weatherModelRounder), jsonCoder(_jsonCoder), sdCardStorage(_sdCardStorage) {}

bool FirestoreWeatherRepository::sendWeatherData(WeatherModel weather) {
  bool isSuccessful = client.write(weatherModelRounder.round(weather));
  if (isSuccessful) {
    sendCachedWeathers();
  } else {
    cacheWeather(weather);
  }
  return isSuccessful;
}

bool FirestoreWeatherRepository::sendRainDetected() {
  delay(2000);
  // int resultCode = client.post("/weather/rain-detected");
  Serial.println("Rain detected");
  int resultCode = HTTP_CODE_OK;  // TODO remove mock
  if (resultCode == HTTP_CODE_OK) {
    return true;
  } else {
    return false;
  }
}

void FirestoreWeatherRepository::sendCachedWeathers() {
  std::vector<String> jsonModels = sdCardStorage.readAllInDirectory(CACHED_WEATHERS_PATH);
  if (jsonModels.size() == 0) return;

  String resultJson = jsonCoder.encodeWeathersList(jsonModels);
  // int resultCode = client.post("/weather/cached", resultJson);
  // if (resultCode == HTTP_CODE_CREATED) {
  //   sdCardStorage.removeAllInDirectory(CACHED_WEATHERS_PATH);
  // }
}

void FirestoreWeatherRepository::cacheWeather(WeatherModel weather) {
  String fileName = getFileNameToCacheWeather(weather.timestamp);
  String json = jsonCoder.encodeWeather(weather);
  sdCardStorage.write(fileName, json);
}

String FirestoreWeatherRepository::getFileNameToCacheWeather(String formattedDate) {
  String timestampWithoutInvalidChars = String(formattedDate);
  timestampWithoutInvalidChars.replace(':', '-');
  timestampWithoutInvalidChars.replace(".000", "");
  return CACHED_WEATHERS_PATH + "/" + timestampWithoutInvalidChars + TXT_EXT;
}