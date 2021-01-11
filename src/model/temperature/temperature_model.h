#ifndef TEMPERATURE_MODEL_H
#define TEMPERATURE_MODEL_H

struct TemperatureModel {
  float temperature;
  float humidity;
  bool hasError;

  TemperatureModel(float _temperature, float _humidity)
      : temperature(_temperature), humidity(_humidity), hasError(false) {}

  TemperatureModel() : temperature(-1), humidity(-1), hasError(true) {}

  TemperatureModel static error() { return TemperatureModel(); }
};

#endif