#include "weather_repository.h"

WeatherRepository::WeatherRepository(RestClient& _client,
                                     JsonEncoder& _jsonEncoder)
    : client(_client), jsonEncoder(_jsonEncoder) {}

int WeatherRepository::sendWeatherData(WeatherModel model) {
  String json = jsonEncoder.encodeWeatherModel(model);
  return client.post("/weather/current", json);
}