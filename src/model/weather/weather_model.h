#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

#include "../air_quality/air_quality_model.h"
#include "../temperature/temperature_model.h"
#include "../wind/wind_model.h"

struct WeatherModel {
  TemperatureModel temperature;
  AirQualityModel airQuality;
  WindModel wind;

  WeatherModel(TemperatureModel _temperature, AirQualityModel _airQuality,
               WindModel _wind)
      : temperature(_temperature), airQuality(_airQuality), wind(_wind) {}
};

#endif