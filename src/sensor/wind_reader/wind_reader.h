#ifndef WIND_READER_H
#define WIND_READER_H

#define SINGE_MEASURE_SECONDS 3

#include <Arduino.h>

#include "../../model/wind/wind_model.h"
#include "../periodic_sensor_reader.h"

class WindReader : public PeriodicSensorReader<WindModel> {
 public:
  WindReader(uint8_t _windSensorPin);

  void begin() override;
  bool startReading() override;
  bool stopReading() override;
  WindModel getData() override;
  String getErrorMessage() override;

 private:
  uint8_t windSensorPin;

  int rotations;
  int interval;
  unsigned long bounceTime;

  float windSpeedMin;
  float windSpeedMax;
  float windSpeedAvg;

  String errorMessage;

  void ICACHE_RAM_ATTR countRotations();
  float calculateWindSpeed();
  void updateWindsSpeed(float wind);
};

#endif