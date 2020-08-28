#ifndef PRESSURE_MODEL_H
#define PRESSURE_MODEL_H

struct PressureModel {
  float temperature;
  float pressure;
  bool hasError;

  PressureModel(float _temperature, float _pressure)
      : temperature(_temperature), pressure(_pressure), hasError(false) {}

  PressureModel() : temperature(-1), pressure(-1), hasError(true) {}

  PressureModel static error() { return PressureModel(); }
};

#endif