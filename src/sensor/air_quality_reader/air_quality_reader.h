#ifndef AIR_QUALITY_READER_H
#define AIR_QUALITY_READER_H

#include <Arduino.h>
#include <PMserial.h>

#include "../../model/air_quality/air_quality_model.h"
#include "../sensor_reader.h"

class AirQualityReader : public SensorReader<AirQualityModel> {
 public:
  AirQualityReader(HardwareSerial &serial);

  void begin() override;
  bool read() override;
  AirQualityModel getData() override;
  String getErrorMessage() override;

 private:
  SerialPM pms;
};

#endif