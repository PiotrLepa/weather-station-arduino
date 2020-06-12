#ifndef RAIN_GAUGE_READER_H
#define RAIN_GAUGE_READER_H

#define ROTATION_DEBOUNCE_TIME 15
#define ML_FOR_TIP 2.5

#include <Arduino.h>
#include <Ticker.h>

#include <vector>

#include "../../model/rain_gauge/rain_gauge_model.h"
#include "../sensor_reader.h"

class RainGaugeReader {
 public:
  RainGaugeReader(uint8_t _rainGaugeSensorPin);

  void begin();
  void startReading();
  void stopReading();
  RainGaugeModel getData();
  String getErrorMessage();

  void ICACHE_RAM_ATTR countTips();

 private:
  uint8_t rainGaugeSensorPin;

  int tips;
  unsigned long bounceTime;
  int measurementCounter;

  String errorMessage;
};

#endif