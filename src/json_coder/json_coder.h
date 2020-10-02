#ifndef JSON_CODER_H
#define JSON_CODER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../model/weather/weather_model.h"
#include "../model/wifi_name/wifi_name_model.h"

#ifdef JSON_CODER_DEBUG
#define JSON_CODER_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifndef JSON_CODER_DEBUG
#define JSON_CODER_DEBUG_PRINT(string)
#endif

class JsonCoder {
 public:
  String encodeWeatherModel(WeatherModel model);
  String encodeWifiNameList(std::vector<WifiNameModel> models);

 private:
  double formatTemperature(TemperatureModel temp1, PressureModel temp2);
  double formatPressure(PressureModel model);
};

#endif