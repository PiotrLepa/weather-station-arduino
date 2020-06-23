#ifndef PRESSURE_MODEL_H
#define PRESSURE_MODEL_H

struct PressureModel {
  float temperature;
  float pressure;

  PressureModel(float _temperature, float _pressure)
      : temperature(_temperature), pressure(_pressure) {}

  PressureModel() : temperature(-1), pressure(-1) {}
};

#endif