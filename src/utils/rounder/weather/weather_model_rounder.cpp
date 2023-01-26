#include "weather_model_rounder.h"

WeatherModel WeatherModelRounder::round(WeatherModel model) {
  return WeatherModel(roundExternalTemperatureModel(model.externalTemperature), roundPressureModel(model.pressure),
                      roundAirQualityModel(model.airQuality), roundWindModel(model.wind), roundRainGaugeModel(model.rainGauge),
                      model.timestamp);
}

ExternalTemperatureModel WeatherModelRounder::roundExternalTemperatureModel(ExternalTemperatureModel model) {
  return ExternalTemperatureModel(numberRounder.roundToOneDecimalPlace(model.temperature));
}

PressureModel WeatherModelRounder::roundPressureModel(PressureModel model) {
  return PressureModel(numberRounder.roundToOneDecimalPlace(model.temperature), numberRounder.roundToOneDecimalPlace(model.pressure),
                       numberRounder.roundToOneDecimalPlace(model.humidity));
}

AirQualityModel WeatherModelRounder::roundAirQualityModel(AirQualityModel model) {
  return AirQualityModel(numberRounder.roundToOneDecimalPlace(model.pm1), numberRounder.roundToOneDecimalPlace(model.pm25),
                         numberRounder.roundToOneDecimalPlace(model.pm10));
}

WindModel WeatherModelRounder::roundWindModel(WindModel model) {
  return WindModel(numberRounder.roundToOneDecimalPlace(model.windSpeedMax), numberRounder.roundToOneDecimalPlace(model.windSpeedAvg));
}

RainGaugeModel WeatherModelRounder::roundRainGaugeModel(RainGaugeModel model) {
  return RainGaugeModel(numberRounder.roundToOneDecimalPlace(model.precipitation));
}