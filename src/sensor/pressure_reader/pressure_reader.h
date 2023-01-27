#ifndef PRESSURE_READER_H
#define PRESSURE_READER_H

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

#include "../../model/pressure/pressure_model.h"
#include "../sensor_reader.h"

class PressureReader : public SensorReader<PressureModel> {
 public:
  PressureReader();

  bool begin() override;
  bool read() override;
  PressureModel getData() override;
  String getErrorMessage() override;

 private:
  Adafruit_BME280 bme;
  PressureModel readModel;
  String errorMessage;
};

#endif