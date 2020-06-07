#ifndef PM_CONTROLLER_H
#define PM_CONTROLLER_H

#include <Arduino.h>
#include <PMserial.h>

#include "../model/air_quality/air_quality_model.h"

class PmController {
 public:
  PmController(HardwareSerial &serial);
  void begin();
  bool read();
  AirQualityModel getData();
  String getErrorMessage();

 private:
  SerialPM pms;
};

#endif