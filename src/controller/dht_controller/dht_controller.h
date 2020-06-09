#ifndef DHT_CONTROLLER_H
#define DHT_CONTROLLER_H

#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>

#include "../../model/temperature/temperature_model.h"

class DhtController {
 public:
  DhtController(uint8_t pin);
  void begin();
  bool read();
  TemperatureModel getData();
  String getErrorMessage();

 private:
  DHT dht;
  TemperatureModel readModel;
  String errorMessage;
};

#endif