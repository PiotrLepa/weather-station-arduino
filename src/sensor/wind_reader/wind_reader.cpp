#include "wind_reader.h"

WindReader* windReaderInstance = NULL;

void handleWindSendorPinInterrupt() { windReaderInstance->countRotations(); }

void calculate() { windReaderInstance->updateWindSpeed(); }

WindReader::WindReader(uint8_t _windSensorPin)
    : windSensorPin(_windSensorPin), timer(calculate, 5000), status(INACTIVE), rotations(0), bounceTime(0), measurementCounter(0) {
  windReaderInstance = this;
}

bool WindReader::begin() { return true; }

void WindReader::update() { timer.update(); }

void WindReader::startReading() {
  status = ACTIVE;
  measurementCounter = 0;
  windSpeedMax = 0;
  rotations = 0;
  windSpeeds.clear();

  timer.start();
  attachInterrupt(digitalPinToInterrupt(windSensorPin), handleWindSendorPinInterrupt, RISING);
}

void WindReader::stopReading() {
  status = INACTIVE;
  detachInterrupt(digitalPinToInterrupt(windSensorPin));
}

void WindReader::countRotations() {
  if (status == ACTIVE && (millis() - bounceTime) > ROTATION_DEBOUNCE_TIME) {
    rotations++;
    bounceTime = millis();
  }
}

void WindReader::updateWindSpeed() {
  measurementCounter++;

  status = COMPUTING;

  float wind = calculateWindSpeed();

  if (windSpeedMax == -1 || wind > windSpeedMax) {
    windSpeedMax = wind;
  }

  windSpeeds.push_back(wind);

  rotations = 0;

  if (status == COMPUTING) {
    status = ACTIVE;
  }
}

float WindReader::calculateWindSpeed() { return (float)rotations / (float)SINGE_MEASURE_SECONDS * ROTATION_TO_WIND_SPEED_CONST; }

WindModel WindReader::getData() {
  float sum = std::accumulate(windSpeeds.begin(), windSpeeds.end(), decltype(windSpeeds)::value_type(0));
  return WindModel(windSpeedMax, sum / measurementCounter);
}