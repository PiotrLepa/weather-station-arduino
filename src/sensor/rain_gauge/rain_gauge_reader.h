#ifndef RAIN_GAUGE_READER_H
#define RAIN_GAUGE_READER_H

#define ROTATION_DEBOUNCE_TIME 15
#define ML_FOR_TIP 2.5
#define RAIN_DETECTOR_DELAY_SECONDS 1800

#include <Arduino.h>
#include <Ticker.h>

#include <vector>

#include "../../model/rain_gauge/rain_gauge_model.h"
#include "../../utils/date_time/date_time.h"
#include "../continuous_sensor_reader.h"
#include "rain_gauge_callbacks.h"

class RainGaugeReader : public ContinuousSensorReader<RainGaugeModel> {
 public:
  RainGaugeReader(uint8_t _rainGaugeSensorPin);

  bool begin() override;
  void update() override;
  void startReading() override;
  void stopReading() override;
  RainGaugeModel getData() override;
  void setCallback(RainGaugeCallbacks* callback);

  void countTips();

 private:
  uint8_t rainGaugeSensorPin;
  long lastRainfallTime;
  RainGaugeCallbacks* callback;

  int tips;
  unsigned long bounceTime;

  void handleRainDetector();
};

#endif