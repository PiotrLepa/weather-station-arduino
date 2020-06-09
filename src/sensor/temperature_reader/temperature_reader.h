#ifndef TEMPERATURE_READER_H
#define TEMPERATURE_READER_H

#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>

#include "../../model/temperature/temperature_model.h"
#include "../sensor_reader.h"

class TemperatureReader : public SensorReader<TemperatureModel> {
 public:
  TemperatureReader(uint8_t pin);

  void begin() override;
  bool read() override;
  TemperatureModel getData() override;
  String getErrorMessage() override;

 private:
  DHT dht;
  TemperatureModel readModel;
  String errorMessage;
};

#endif