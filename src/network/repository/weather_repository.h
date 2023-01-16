#ifndef WEATHER_REPOSITORY_H
#define WEATHER_REPOSITORY_H

#include <HTTPClient.h>

#include "../../model/cached_weather/cached_weather_model.h"
#include "../../model/weather/weather_model.h"
#include "../../storage/sd_card/sd_card_storage.h"
#include "../../utils/date_time/date_time.h"
#include "../../utils/json_coder/json_coder.h"
#include "../rest_client/rest_client.h"

class WeatherRepository {
 public:
  WeatherRepository(RestClient& _client, JsonCoder& _jsonCoder, SdCardStorage& _sdCardStorage);

  bool sendWeatherData(WeatherModel weather);
  bool sendRainDetected();

 private:
  RestClient& client;
  JsonCoder& jsonCoder;
  SdCardStorage& sdCardStorage;

  void cacheWeather(WeatherModel weather);
  void sendCachedWeathers();
  String getFileNameToCacheWeather(String formattedDate);
};

#endif