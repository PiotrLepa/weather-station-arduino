#ifndef AIR_QUALITY_READER_H
#define AIR_QUALITY_READER_H

#include <Arduino.h>
#include <PMserial.h>

#define PMS_WAKE_UP_MILLIS 30000

#include "../../model/air_quality/air_quality_model.h"
#include "../sensor_reader.h"

class AirQualityReader : public SensorReader<AirQualityModel> {
 public:
  AirQualityReader(HardwareSerial &serial, uint8_t _pmsModePin);

  void begin() override;
  bool read() override;
  AirQualityModel getData() override;
  String getErrorMessage() override;
  void wakeUp();
  void sleep();

 private:
  SerialPM pms;
  uint8_t pmsModePin;
};

#endif