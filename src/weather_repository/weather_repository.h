#ifndef WEATHER_REPOSITORY_H
#define WEATHER_REPOSITORY_H

#include "../date_time/date_time.h"
#include "../json_coder/json_coder.h"
#include "../model/cached_weather/cached_weather_model.h"
#include "../model/weather/weather_model.h"
#include "../rest_client/rest_client.h"
#include "../sd_card/sd_card_manager.h"

class WeatherRepository {
 public:
  WeatherRepository(RestClient& _client, JsonCoder& _jsonCoder,
                    SdCardManager& _sdCardManager, DateTime& _dateTime);

  bool sendWeatherData(WeatherModel weather);

 private:
  RestClient& client;
  JsonCoder& jsonCoder;
  SdCardManager& sdCardManager;
  DateTime& dateTime;
};

#endif