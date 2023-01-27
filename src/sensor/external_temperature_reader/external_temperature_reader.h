#ifndef EXTERNAL_TEMPERATURE_READER_H
#define EXTERNAL_TEMPERATURE_READER_H

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include "../../model/external_temperature/external_temperature_model.h"
#include "../sensor_reader.h"

class ExternalTemperatureReader : public SensorReader<ExternalTemperatureModel> {
 public:
  ExternalTemperatureReader(OneWire* oneWire);

  bool begin() override;
  ExternalTemperatureModel read() override;

 private:
  DallasTemperature sensor;
};

#endif