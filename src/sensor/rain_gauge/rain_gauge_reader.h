#ifndef RAIN_GAUGE_READER_H
#define RAIN_GAUGE_READER_H

#define ROTATION_DEBOUNCE_TIME 15
#define ML_FOR_TIP 2.5
#define RAIN_DETECTOR_DELAY_SECONDS 3600

#include <Arduino.h>
#include <Ticker.h>

#include <vector>

#include "../../model/rain_gauge/rain_gauge_model.h"
#include "../../utils/date_time/date_time.h"
#include "../continuous_sensor_reader.h"

class RainGaugeReader : public ContinuousSensorReader<RainGaugeModel> {
 public:
  RainGaugeReader(uint8_t _rainGaugeSensorPin);

  void begin() override;
  void startReading() override;
  void stopReading() override;
  RainGaugeModel getData() override;
  String getErrorMessage() override;
  void setRainDetectedCallback(void (*callback)());

  void ICACHE_RAM_ATTR countTips();

 private:
  uint8_t rainGaugeSensorPin;
  long rainDetectorLastTriggerTime;
  void (*rainDetectedCallback)();

  int tips;
  unsigned long bounceTime;

  String errorMessage;

  void handleRainDetector();
};

#endif