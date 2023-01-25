#include "firestore_weather_repository.h"

FirestoreWeatherRepository::FirestoreWeatherRepository(RestClient& _client, JsonCoder& _jsonCoder, SdCardStorage& _sdCardStorage)
    : client(_client), jsonCoder(_jsonCoder), sdCardStorage(_sdCardStorage) {}

bool FirestoreWeatherRepository::sendWeatherData(WeatherModel weather) {
  String json = jsonCoder.encodeWeatherModel(weather);
  // int resultCode = client.post("/weather", json);
  int resultCode = 500; // TODO remove mock
  if (resultCode == HTTP_CODE_CREATED) {
    sendCachedWeathers();
    return true;
  } else {
    cacheWeather(weather);
    return false;
  }
}

bool FirestoreWeatherRepository::sendRainDetected() {
  delay(2000);
  // int resultCode = client.post("/weather/rain-detected");
  Serial.println("Rain detected");
  int resultCode = HTTP_CODE_OK; // TODO remove mock
  if (resultCode == HTTP_CODE_OK) {
    return true;
  } else {
    return false;
  }
}

void FirestoreWeatherRepository::sendCachedWeathers() {
  std::vector<String> jsonModels = sdCardStorage.readAllInDirectory(CACHED_WEATHERS_PATH);
  if (jsonModels.size() == 0) return;

  String resultJson = jsonCoder.encodeCachedWeathersList(jsonModels);
  int resultCode = client.post("/weather/cached", resultJson);
  if (resultCode == HTTP_CODE_CREATED) {
    sdCardStorage.removeAllInDirectory(CACHED_WEATHERS_PATH);
  }
}

void FirestoreWeatherRepository::cacheWeather(WeatherModel weather) {
  DateTime dateTime = DateTime::now();
  if (dateTime.getSecondsFromEpoch() == -1) return;

  String formattedDate = dateTime.getFormattedDate();
  CachedWeatherModel cachedWeatherModel = CachedWeatherModel(weather, formattedDate);

  String fileName = getFileNameToCacheWeather(formattedDate);
  String json = jsonCoder.encodeCachedWeatherModel(cachedWeatherModel);
  // sdCardStorage.write(fileName, json); // TODO enable
}

String FirestoreWeatherRepository::getFileNameToCacheWeather(String formattedDate) {
  String timestampWithoutInvalidChars = String(formattedDate);
  timestampWithoutInvalidChars.replace(':', '-');
  timestampWithoutInvalidChars.replace(".000", "");
  return CACHED_WEATHERS_PATH + "/" + timestampWithoutInvalidChars + TXT_EXT;
}