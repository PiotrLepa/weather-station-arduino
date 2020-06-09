#ifndef DHT_CONTROLLER_H
#define DHT_CONTROLLER_H

#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>

#include "../../model/temperature/temperature_model.h"
#include "../sensor_controller.h"

class DhtController : public SensorController<TemperatureModel> {
 public:
  DhtController(uint8_t pin);

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