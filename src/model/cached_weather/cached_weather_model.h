#ifndef CACHED_WEATHER_MODEL_H
#define CACHED_WEATHER_MODEL_H

#include "../air_quality/air_quality_model.h"
#include "../pressure/pressure_model.h"
#include "../rain_gauge/rain_gauge_model.h"
#include "../temperature/temperature_model.h"
#include "../weather/weather_model.h"
#include "../wind/wind_model.h"
#include "Arduino.h"

struct CachedWeatherModel {
  WeatherModel weather;
  String timestamp;

  CachedWeatherModel(WeatherModel _weather, String _timestamp)
      : weather(_weather), timestamp(_timestamp) {}
};

#endif