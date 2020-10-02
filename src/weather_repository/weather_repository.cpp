#include "weather_repository.h"

WeatherRepository::WeatherRepository(RestClient& _client,
                                     JsonCoder& _jsonCoder)
    : client(_client), jsonCoder(_jsonCoder) {}

int WeatherRepository::sendWeatherData(WeatherModel model) {
  String json = jsonCoder.encodeWeatherModel(model);
  return client.post("/weather/current", json);
}