#ifndef WEATHER_REPOSITORY_H

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
  WeatherRepository(RestClient& _client);

  int sendWeatherData(String path, WeatherModel model);

 private:
  RestClient& client;
};

#endif