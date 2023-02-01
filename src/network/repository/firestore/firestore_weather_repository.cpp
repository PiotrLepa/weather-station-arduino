#include "firestore_weather_repository.h"

FirestoreWeatherRepository::FirestoreWeatherRepository(FirestoreClient& _client, JsonCoder& _jsonCoder, SdCardStorage& _sdCardStorage)
    : client(_client), jsonCoder(_jsonCoder), sdCardStorage(_sdCardStorage) {}

bool FirestoreWeatherRepository::sendWeatherData(WeatherModel weather) {
  bool isSuccessful = client.write(weather);
  if (isSuccessful) {
    // sendCachedWeathers();  TODO enable when ready
  } else {
    cacheWeather(weather);
  }
  return isSuccessful;
}

bool FirestoreWeatherRepository::sendRainDetected() {
  // TODO invoke firebase clound function
  Serial.println("Rain detected");

  LOGGER.log("Sending rain detection to firebase failed");
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
  } else {
    LOGGER.log("Sending cached weathers to firebase failed");
  }
}

void FirestoreWeatherRepository::cacheWeather(WeatherModel weather) {
  String fileName = getFileNameToCacheWeather(weather.timestamp);
  String json = jsonCoder.encodeWeather(weather);
  bool weatherSaved = sdCardStorage.write(fileName, json);
  if (!weatherSaved) {
    LOGGER.log("Save weather to sd card failed");
  }
}

String FirestoreWeatherRepository::getFileNameToCacheWeather(String formattedDate) {
  String timestampWithoutInvalidChars = String(formattedDate);
  timestampWithoutInvalidChars.replace(':', '-');
  timestampWithoutInvalidChars.replace(".000", "");
  return CACHED_WEATHERS_PATH + "/" + timestampWithoutInvalidChars + TXT_EXT;
}