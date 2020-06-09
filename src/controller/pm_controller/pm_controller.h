#ifndef PM_CONTROLLER_H
#define PM_CONTROLLER_H

#include <Arduino.h>
#include <PMserial.h>

#include "../../model/air_quality/air_quality_model.h"
#include "../sensor_controller.h"

class PmController : public SensorController<AirQualityModel> {
 public:
  PmController(HardwareSerial &serial);

  void begin() override;
  bool read() override;
  AirQualityModel getData() override;
  String getErrorMessage() override;

 private:
  SerialPM pms;
};

#endif