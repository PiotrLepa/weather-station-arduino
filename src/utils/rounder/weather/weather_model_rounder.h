#ifndef WEATHER_MODEL_ROUNDER_H
#define WEATHER_MODEL_ROUNDER_H

#include <Arduino.h>

#include "../../../model/weather/weather_model.h"
#include "../number/number_rounder.h"

class WeatherModelRounder {
 public:
  WeatherModel round(WeatherModel model);

 private:
  NumberRounder numberRounder;
  
  ExternalTemperatureModel roundExternalTemperatureModel(ExternalTemperatureModel model);
  PressureModel roundPressureModel(PressureModel model);
  AirQualityModel roundAirQualityModel(AirQualityModel model);
  WindModel roundWindModel(WindModel model);
  RainGaugeModel roundRainGaugeModel(RainGaugeModel model);
};

#endif