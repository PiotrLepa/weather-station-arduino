#include "firestore_weather_repository.h"

FirestoreWeatherRepository::FirestoreWeatherRepository(FirestoreClient& _client, JsonCoder& _jsonCoder, SdCardStorage& _sdCardStorage)
    : client(_client), jsonCoder(_jsonCoder), sdCardStorage(_sdCardStorage) {}

bool FirestoreWeatherRepository::sendWeatherData(WeatherModel weather) {
  bool isSuccessful = client.write(weather);
  if (isSuccessful) {
    sendCachedWeathers();
  } else {
    cacheWeather(weather);
  }
  return isSuccessful;
}

bool FirestoreWeatherRepository::sendRainDetected() {
  // TODO invoke firebase clound function
  Serial.println("Rain detected");
  return false;
}

void FirestoreWeatherRepository::sendCachedWeathers() {
  std::vector<String> jsonModels = sdCardStorage.readAllInDirectory(CACHED_WEATHERS_PATH);
  if (jsonModels.size() == 0) return;

  String resultJson = jsonCoder.encodeWeathersList(jsonModels);
  // TODO send cached weathers to firebase
  bool isSuccessful = false;
  if (isSuccessful) {
    sdCardStorage.removeAllInDirectory(CACHED_WEATHERS_PATH);
  }
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