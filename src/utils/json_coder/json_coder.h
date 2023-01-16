#ifndef JSON_CODER_H
#define JSON_CODER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../../model/cached_weather/cached_weather_model.h"
#include "../../model/weather/weather_model.h"

class JsonCoder {
 public:
  String encodeWeatherModel(WeatherModel model);
  String encodeCachedWeatherModel(CachedWeatherModel model);
  String encodeCachedWeathersList(std::vector<String> jsonModels);

 private:
  void printJson(JsonDocument &source);
  void printJson(JsonArray &source);
  double formatToOneDecimalPoint(double value);
  double formatTemperature(ExternalTemperatureModel model);
  double formatHumidity(PressureModel model);
};

#endif