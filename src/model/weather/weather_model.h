#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

#include "../air_quality/air_quality_model.h"
#include "../temperature/temperature_model.h"

struct WeatherModel {
  TemperatureModel temperature;
  AirQualityModel airQuality;

  WeatherModel(TemperatureModel _temperature, AirQualityModel _airQuality)
      : temperature(_temperature), airQuality(_airQuality) {}
};

#endif