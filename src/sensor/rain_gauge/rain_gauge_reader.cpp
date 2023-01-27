#include "rain_gauge_reader.h"

RainGaugeReader* rainGaugeReaderInstance = NULL;

void IRAM_ATTR handleRainGaugePinInterrupt() { rainGaugeReaderInstance->countTips(); }

RainGaugeReader::RainGaugeReader(uint8_t _rainGaugeSensorPin)
    : rainGaugeSensorPin(_rainGaugeSensorPin), lastRainfallTime(0), tips(0), bounceTime(0) {
  rainGaugeReaderInstance = this;
  pinMode(rainGaugeSensorPin, INPUT);
}

bool RainGaugeReader::begin() {return true;}

void RainGaugeReader::update() { handleRainDetector(); }

void RainGaugeReader::startReading() {
  tips = 0;
  attachInterrupt(digitalPinToInterrupt(rainGaugeSensorPin), handleRainGaugePinInterrupt, RISING);
}

void RainGaugeReader::stopReading() { detachInterrupt(digitalPinToInterrupt(rainGaugeSensorPin)); }

void RainGaugeReader::countTips() {
  if ((millis() - bounceTime) > ROTATION_DEBOUNCE_TIME) {
    tips++;
    bounceTime = millis();
  }
}

RainGaugeModel RainGaugeReader::getData() { return RainGaugeModel(tips * ML_FOR_TIP); }

void RainGaugeReader::setCallback(RainGaugeCallbacks* _callback) { callback = _callback; }

void RainGaugeReader::handleRainDetector() {
  if (tips < 3) return;

  long now = DateTime::now().getSecondsFromEpoch();
  if (now == -1) return;

  if (lastRainfallTime == 0 || now - lastRainfallTime >= RAIN_DETECTOR_DELAY_SECONDS) {
    callback->rainDetected();
  }

  lastRainfallTime = now;
}