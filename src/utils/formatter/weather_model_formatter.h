#ifndef WEATHER_MODEL_FORMATTER_H
#define WEATHER_MODEL_FORMATTER_H

#include <Arduino.h>

#include "../../model/weather/weather_model.h"

class WeatherModelFormatter {
 public:
  String format(WeatherModel weather);
};

#endif