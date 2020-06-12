#ifndef TEMPERATURE_MODEL_H
#define TEMPERATURE_MODEL_H

struct TemperatureModel {
  float temperature;
  float humidity;

  TemperatureModel(float _temperature, float _humidity)
      : temperature(_temperature), humidity(_humidity) {}

  TemperatureModel() : temperature(-1), humidity(-1) {}
};

#endif