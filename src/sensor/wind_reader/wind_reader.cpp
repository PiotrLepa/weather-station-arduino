#include "wind_reader.h"

WindReader* instance = NULL;

void ICACHE_RAM_ATTR handlePinInterrupt() { instance->countRotations(); }

void calculate() { instance->updateWindSpeed(); }

WindReader::WindReader(uint8_t _windSensorPin)
    : windSensorPin(_windSensorPin),
      timer(calculate, 5000),
      status(INACTIVE),
      rotations(0),
      interval(0),
      bounceTime(0),
      measurementCounter(0),
      errorMessage("No errors") {
  instance = this;
}

void WindReader::begin() {}

void WindReader::startReading() {
  status = ACTIVE;
  measurementCounter = 0;
  windSpeedMax = -1;
  windSpeeds.clear();
  errorMessage = "No errors";

  measurementCounter++;
  rotations = 0;

  timer.start();
  attachInterrupt(digitalPinToInterrupt(windSensorPin), handlePinInterrupt,
                  RISING);
}

void WindReader::stopReading() {
  status = INACTIVE;
  detachInterrupt(digitalPinToInterrupt(windSensorPin));
}

void WindReader::update() { timer.update(); }

void ICACHE_RAM_ATTR WindReader::countRotations() {
  if (status == ACTIVE && (millis() - bounceTime) > 15) {
    Serial.println(rotations);
    rotations++;
    bounceTime = millis();
  }
}

void WindReader::updateWindSpeed() {
  Serial.println("updateWindSpeed");
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

float WindReader::calculateWindSpeed() {
  return (float)rotations / (float)SINGE_MEASURE_SECONDS * 2.4;
}

WindModel WindReader::getData() {
  float sum = std::accumulate(windSpeeds.begin(), windSpeeds.end(),
                              decltype(windSpeeds)::value_type(0));
  return WindModel(windSpeedMax, sum / measurementCounter);
}

String WindReader::getErrorMessage() { return errorMessage; }