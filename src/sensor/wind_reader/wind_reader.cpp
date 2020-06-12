#include "wind_reader.h"

WindReader::WindReader(uint8_t _windSensorPin)
    : windSensorPin(_windSensorPin),
      rotations(0),
      interval(0),
      bounceTime(0),
      errorMessage("No errors") {}

void WindReader::begin() {}

bool WindReader::startReading() {
  rotations = 0;

  attachInterrupt(digitalPinToInterrupt(windSensorPin), countRotations, RISING);
  delay(SINGE_MEASURE_SECONDS * 1000);
  detachInterrupt(digitalPinToInterrupt(windSensorPin));

  float wind = calculateWindSpeed();
  updateWindsSpeed(wind);
}

bool WindReader::stopReading() {}

WindModel WindReader::getData() {
  return WindModel(windSpeedMax, windSpeedMin, windSpeedAvg);
}

String WindReader::getErrorMessage() { return errorMessage; }

void ICACHE_RAM_ATTR WindReader::countRotations() {
  if ((millis() - bounceTime) > 15) {
    rotations++;
    bounceTime = millis();
  }
}

float WindReader::calculateWindSpeed() {
  return (float)rotations / (float)SINGE_MEASURE_SECONDS * 2.4;
}

void WindReader::updateWindsSpeed(float wind) {
  if (wind < windSpeedMin) {
    windSpeedMin = wind;
  }

  if (wind > windSpeedMax) {
    windSpeedMax = wind;
  }
}