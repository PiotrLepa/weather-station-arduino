#ifndef WEATHER_REPOSITORY_H
#define WEATHER_REPOSITORY_H

#include "../../model/weather/weather_model.h"

class WeatherRepository
{
public:
  virtual bool sendWeatherData(WeatherModel weather) = 0;
  virtual bool sendRainDetected() = 0;
};

#endif