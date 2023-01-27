#ifndef WIND_READER_H
#define WIND_READER_H

#define SINGE_MEASURE_SECONDS 5
#define ROTATION_DEBOUNCE_TIME 15
#define ROTATION_TO_WIND_SPEED_CONST 2.4

#include <Arduino.h>
#include <Ticker.h>
#include <numeric>

#include <vector>

#include "../../model/wind/wind_model.h"
#include "../continuous_sensor_reader.h"

class WindReader : public ContinuousSensorReader<WindModel> {
 public:
  WindReader(uint8_t _windSensorPin);

  bool begin() override;
  void update() override;
  void startReading() override;
  void stopReading() override;
  WindModel getData() override;
  String getErrorMessage() override;
  void updateWindSpeed();

  void countRotations();

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
  unsigned long bounceTime;
  int measurementCounter;

  float windSpeedMax;
  std::vector<float> windSpeeds;

  String errorMessage;

  float calculateWindSpeed();
};

#endif