#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

#include "../air_quality/air_quality_model.h"
#include "../external_temperature/external_temperature_model.h"
#include "../pressure/pressure_model.h"
#include "../rain_gauge/rain_gauge_model.h"
#include "../temperature/temperature_model.h"
#include "../wind/wind_model.h"

struct WeatherModel {
  ExternalTemperatureModel externalTemperature;
  PressureModel pressure;
  AirQualityModel airQuality;
  WindModel wind;

  RainGaugeModel rainGauge;

  WeatherModel(ExternalTemperatureModel _externalTemperature, PressureModel _pressure, AirQualityModel _airQuality,
               WindModel _wind, RainGaugeModel _rainGauge)
      : externalTemperature(_externalTemperature),
        pressure(_pressure),
        airQuality(_airQuality),
        wind(_wind),
        rainGauge(_rainGauge) {}

  bool canBeSendToServer() {
    if (externalTemperature.hasError) {
      Serial.println("External temperature error");
    }
    if (pressure.hasError) {
      Serial.println("Pressure error");
    }
    if (airQuality.hasError) {
      Serial.println("Air quality error");
    }
    if (wind.hasError) {
      Serial.println("Wind error");
    }
    if (rainGauge.hasError) {
      Serial.println("Rain gauge error");
    }
    return !externalTemperature.hasError && !pressure.hasError && !airQuality.hasError && !wind.hasError &&
           !rainGauge.hasError;
  }
};

#endif