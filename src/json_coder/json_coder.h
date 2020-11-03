#ifndef JSON_CODER_H
#define JSON_CODER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../model/cached_weather/cached_weather_model.h"
#include "../model/weather/weather_model.h"
#include "../model/wifi/wifi_model.h"
#include "../model/wifi_credentials/wifi_credentials_model.h"

class JsonCoder {
 public:
  String encodeWeatherModel(WeatherModel model);
  String encodeCachedWeatherModel(CachedWeatherModel model);
  String encodeWifiNameList(std::vector<WifiModel> models);
  WifiCredentialsModel decodeWifiCredentials(String json);

 private:
  void printJson(JsonDocument &source);
  void printJson(JsonArray &source);
  double formatTemperature(TemperatureModel temp1, PressureModel temp2);
  double formatPressure(PressureModel model);
};

#endif