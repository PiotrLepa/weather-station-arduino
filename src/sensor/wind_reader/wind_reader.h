#ifndef WIND_READER_H
#define WIND_READER_H

#define SINGE_MEASURE_SECONDS 5

#include <Arduino.h>
#include <Ticker.h>

#include <vector>

#include "../../model/wind/wind_model.h"
#include "../sensor_reader.h"

class WindReader {
 public:
  WindReader(uint8_t _windSensorPin);

  void begin();
  void startReading();
  void stopReading();
  void update();
  void updateWindSpeed();
  WindModel getData();
  String getErrorMessage();

  void ICACHE_RAM_ATTR countRotations();

  enum Status {
    ACTIVE,
    INACTIVE,
    COMPUTING,
  };

 private:
  uint8_t windSensorPin;

  Ticker timer;
  Status status;
  int rotations;
  int interval;
  unsigned long bounceTime;
  int measurementCounter;

  float windSpeedMax;
  std::vector<float> windSpeeds;

  String errorMessage;

  float calculateWindSpeed();
};

#endif