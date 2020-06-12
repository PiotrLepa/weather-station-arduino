#include "rain_gauge_reader.h"

RainGaugeReader* rainGaugeReaderInstance = NULL;

void ICACHE_RAM_ATTR handleRainGaugePinInterrupt() {
  rainGaugeReaderInstance->countTips();
}

RainGaugeReader::RainGaugeReader(uint8_t _rainGaugeSensorPin)
    : rainGaugeSensorPin(_rainGaugeSensorPin),
      tips(0),
      bounceTime(0),
      measurementCounter(0),
      errorMessage("No errors") {
  rainGaugeReaderInstance = this;
}

void RainGaugeReader::begin() {}

void RainGaugeReader::startReading() {
  measurementCounter = 0;
  tips = 0;
  errorMessage = "No errors";

  attachInterrupt(digitalPinToInterrupt(rainGaugeSensorPin),
                  handleRainGaugePinInterrupt, RISING);
}

void RainGaugeReader::stopReading() {
  detachInterrupt(digitalPinToInterrupt(rainGaugeSensorPin));
}

void ICACHE_RAM_ATTR RainGaugeReader::countTips() {
  if ((millis() - bounceTime) > ROTATION_DEBOUNCE_TIME) {
    tips++;
    bounceTime = millis();
  }
}

RainGaugeModel RainGaugeReader::getData() {
  return RainGaugeModel(tips * ML_FOR_TIP);
}

String RainGaugeReader::getErrorMessage() { return errorMessage; }