#include "weather_repository.h"

WeatherRepository::WeatherRepository(RestClient& _client) : client(_client) {}

int WeatherRepository::sendWeatherData(String path, WeatherModel model) {
  return client.post("/weather/current", "fs");  // TODO serialize
}