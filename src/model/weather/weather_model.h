#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

#include "../air_quality/air_quality_model.h"
#include "../pressure/pressure_model.h"
#include "../rain_gauge/rain_gauge_model.h"
#include "../temperature/temperature_model.h"
#include "../wind/wind_model.h"

struct WeatherModel
{
  TemperatureModel temperature;
  PressureModel pressure;
  AirQualityModel airQuality;
  WindModel wind;
  RainGaugeModel rainGauge;

  WeatherModel(TemperatureModel _temperature, PressureModel _pressure, AirQualityModel _airQuality, WindModel _wind, RainGaugeModel _rainGauge)
      : temperature(_temperature),
        pressure(_pressure),
        airQuality(_airQuality),
        wind(_wind),
        rainGauge(_rainGauge) {}

  bool canBeSendToServer()
  {
    return !pressure.hasError && !airQuality.hasError && !wind.hasError && !rainGauge.hasError;
  }
};

#endif