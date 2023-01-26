#ifndef WEATHER_PRINTER_H
#define WEATHER_PRINTER_H

#include <Arduino.h>

#include "../../model/weather/weather_model.h"

class WeatherPrinter {
 public:
  void print(WeatherModel weather);
};

#endif