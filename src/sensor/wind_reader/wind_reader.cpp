#include "wind_reader.h"

WindReader* instance = NULL;

void ICACHE_RAM_ATTR handlePinInterrupt() { instance->countRotations(); }

WindReader::WindReader(uint8_t _windSensorPin, int readingSeconds)
    : windSensorPin(_windSensorPin),
      rotations(0),
      interval(0),
      bounceTime(0),
      numberOrMeasurements(0),
      measurementCounter(0),
      errorMessage("No errors") {
  instance = this;
  numberOrMeasurements = readingSeconds / SINGE_MEASURE_SECONDS;
}

void WindReader::begin() {}

bool WindReader::read() {
  measurementCounter = 0;
  windSpeedMax = -1;
  windSpeedMin = -1;
  windSpeeds.clear();
  errorMessage = "No errors";

  measure();

  return true;
}

void WindReader::measure() {
  if (measurementCounter >= numberOrMeasurements) {
    return;
  }

  measurementCounter++;
  rotations = 0;

  attachInterrupt(digitalPinToInterrupt(windSensorPin), handlePinInterrupt,
                  RISING);
  delay(SINGE_MEASURE_SECONDS * 1000);
  detachInterrupt(digitalPinToInterrupt(windSensorPin));

  float wind = calculateWindSpeed();
  updateWindsSpeed(wind);

  measure();
}

void ICACHE_RAM_ATTR WindReader::countRotations() {
  if ((millis() - bounceTime) > 15) {
    Serial.println(rotations);
    rotations++;
    bounceTime = millis();
  }
}

float WindReader::calculateWindSpeed() {
  return (float)rotations / (float)SINGE_MEASURE_SECONDS * 2.4;
}

void WindReader::updateWindsSpeed(float wind) {
  if (windSpeedMin == -1 || wind < windSpeedMin) {
    windSpeedMin = wind;
  }

  if (windSpeedMax == -1 || wind > windSpeedMax) {
    windSpeedMax = wind;
  }

  windSpeeds.push_back(wind);
}

WindModel WindReader::getData() {
  float sum = std::accumulate(windSpeeds.begin(), windSpeeds.end(),
                              decltype(windSpeeds)::value_type(0));
  return WindModel(windSpeedMax, windSpeedMin, sum / numberOrMeasurements);
}

String WindReader::getErrorMessage() { return errorMessage; }