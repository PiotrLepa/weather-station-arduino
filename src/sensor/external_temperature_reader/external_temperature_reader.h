#ifndef EXTERNAL_TEMPERATURE_READER_H
#define EXTERNAL_TEMPERATURE_READER_H

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include "../../model/external_temperature/external_temperature_model.h"
#include "../sensor_reader.h"

class ExternalTemperatureReader : public SensorReader<ExternalTemperatureModel> {
 public:
  ExternalTemperatureReader(uint8_t pin);

  void begin() override;
  bool read() override;
  ExternalTemperatureModel getData() override;
  String getErrorMessage() override;

 private:
  DallasTemperature sensor;
  ExternalTemperatureModel readModel;
  String errorMessage;
};

#endif