#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

#include "../air_quality/air_quality_model.h"
#include "../external_temperature/external_temperature_model.h"
#include "../pressure/pressure_model.h"
#include "../rain_gauge/rain_gauge_model.h"
#include "../temperature/temperature_model.h"
#include "../wind/wind_model.h"
#include "../../utils/date_time/date_time.h"

struct WeatherModel {
  ExternalTemperatureModel externalTemperature;
  PressureModel pressure;
  AirQualityModel airQuality;
  WindModel wind;
  RainGaugeModel rainGauge;
  DateTime timestamp;

  WeatherModel(ExternalTemperatureModel _externalTemperature, PressureModel _pressure, AirQualityModel _airQuality,
               WindModel _wind, RainGaugeModel _rainGauge, DateTime _timestamp)
      : externalTemperature(_externalTemperature),
        pressure(_pressure),
        airQuality(_airQuality),
        wind(_wind),
        rainGauge(_rainGauge),
        timestamp(_timestamp) {}

  bool canBeSendToServer() {
    return !externalTemperature.hasError && !pressure.hasError && !airQuality.hasError && !wind.hasError &&
           !rainGauge.hasError;
  }
};

#endif