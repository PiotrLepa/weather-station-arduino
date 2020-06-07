#ifndef PM_CONTROLLER_H
#define PM_CONTROLLER_H

#include <Arduino.h>
#include <PMserial.h>

#include "../model/pm_model/pm_model.h"

class PmController {
 public:
  PmController(HardwareSerial &serial);
  void begin();
  bool read();
  PmModel getData();
  String getErrorMessage();

 private:
  SerialPM pms;
};

#endif