#include "rain_gauge_reader.h"

RainGaugeReader* rainGaugeReaderInstance = NULL;

void ICACHE_RAM_ATTR handleRainGaugePinInterrupt() { rainGaugeReaderInstance->countTips(); }

RainGaugeReader::RainGaugeReader(uint8_t _rainGaugeSensorPin)
    : rainGaugeSensorPin(_rainGaugeSensorPin),
      rainDetectorLastTriggerTime(0),
      tips(0),
      bounceTime(0),
      errorMessage("No errors") {
  rainGaugeReaderInstance = this;
}

void RainGaugeReader::begin() {}

void RainGaugeReader::startReading() {
  tips = 0;
  errorMessage = "No errors";

  attachInterrupt(digitalPinToInterrupt(rainGaugeSensorPin), handleRainGaugePinInterrupt, RISING);
}

void RainGaugeReader::stopReading() { detachInterrupt(digitalPinToInterrupt(rainGaugeSensorPin)); }

void RainGaugeReader::countTips() {
  if ((millis() - bounceTime) > ROTATION_DEBOUNCE_TIME) {
    tips++;
    bounceTime = millis();
    handleRainDetector();
  }
}

RainGaugeModel RainGaugeReader::getData() { return RainGaugeModel(tips * ML_FOR_TIP); }

String RainGaugeReader::getErrorMessage() { return errorMessage; }

void RainGaugeReader::setRainDetectedCallback(void (*callback)()) { rainDetectedCallback = callback; }

void RainGaugeReader::handleRainDetector() {
  long now = DateTime::now().getSecondsFromEpoch();
  if (now == -1) return;
  if (rainDetectorLastTriggerTime == 0 || now - rainDetectorLastTriggerTime >= RAIN_DETECTOR_DELAY_SECONDS) {
    rainDetectorLastTriggerTime = now;
    rainDetectedCallback();
  }
}