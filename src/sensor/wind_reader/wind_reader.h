#ifndef WIND_READER_H
#define WIND_READER_H

#define SINGE_MEASURE_SECONDS 3

#include <Arduino.h>

#include <vector>

#include "../../model/wind/wind_model.h"
#include "../sensor_reader.h"

class WindReader : public SensorReader<WindModel> {
 public:
  WindReader(uint8_t _windSensorPin, int readingSeconds);

  void begin() override;
  bool read() override;
  WindModel getData() override;
  String getErrorMessage() override;

  void ICACHE_RAM_ATTR countRotations();

 private:
  uint8_t windSensorPin;

  int rotations;
  int interval;
  unsigned long bounceTime;
  int numberOrMeasurements;
  int measurementCounter;

  float windSpeedMin;
  float windSpeedMax;
  std::vector<float> windSpeeds;

  String errorMessage;

  float calculateWindSpeed();
  void updateWindsSpeed(float wind);
  void measure();
};

#endif