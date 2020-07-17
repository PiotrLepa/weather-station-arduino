#ifndef JSON_ENCODER_H
#define JSON_ENCODER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../model/weather/weather_model.h"

#ifdef JSON_ENCODER_DEBUG
#define JSON_ENCODER_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifndef JSON_ENCODER_DEBUG
#define JSON_ENCODER_DEBUG_PRINT(string)
#endif

class JsonEncoder {
 public:
  String encodeWeatherModel(WeatherModel model);

 private:
  double formatTemperature(double temp1, double temp2);
};

#endif