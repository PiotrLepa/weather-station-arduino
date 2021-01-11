#ifndef PRESSURE_MODEL_H
#define PRESSURE_MODEL_H

struct PressureModel {
  float temperature;
  float pressure;
  float humidity;
  bool hasError;

  PressureModel(float _temperature, float _pressure, float _humidity)
      : temperature(_temperature), pressure(_pressure), humidity(_humidity), hasError(false) {}

  PressureModel() : temperature(-1), pressure(-1), humidity(-1), hasError(true) {}

  PressureModel static error() { return PressureModel(); }
};

#endif