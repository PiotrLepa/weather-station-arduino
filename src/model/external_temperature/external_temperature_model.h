#ifndef EXTERNAL_TEMPERATURE_MODEL_H
#define EXTERNAL_TEMPERATURE_MODEL_H

struct ExternalTemperatureModel {
  float temperature;
  bool hasError;

  ExternalTemperatureModel(float _temperature) : temperature(_temperature), hasError(false) {}

  ExternalTemperatureModel() : temperature(-1), hasError(true) {}

  ExternalTemperatureModel static error() { return ExternalTemperatureModel(); }
};

#endif