#ifndef WEATHER_REPOSITORY_H

#include "../json_encoder/json_encoder.h";
#include "../model/weather_model.h";
#include "../rest_client/rest_client.h";

#ifdef WEATHER_REPOSITORY
#define WEATHER_REPOSITORY_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifndef WEATHER_REPOSITORY_DEBUG
#define WEATHER_REPOSITORY_DEBUG_PRINT(string)
#endif

class WeatherRepository {
 public:
  WeatherRepository(RestClient& _client, JsonEncoder& _jsonEncoder);

  int sendWeatherData(String path, WeatherModel model);

 private:
  RestClient& client;
  JsonEncoder& jsonEncoder;
};

#endif