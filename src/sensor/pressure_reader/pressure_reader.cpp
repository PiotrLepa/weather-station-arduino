#include "pressure_reader.h"

PressureReader::PressureReader()
    : bme(), readModel(-1, -1), errorMessage("No errors") {}

void PressureReader::begin() { bme.begin(BME280_ADDRESS_ALTERNATE); }

bool PressureReader::read() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure();

  if (isnan(temperature) || isnan(pressure)) {
    readModel = PressureModel(-1, -1);
    errorMessage = "Failed to read from BME sensor!";
    return false;
  }
  readModel = PressureModel(temperature, pressure);
  return true;
}

PressureModel PressureReader::getData() { return readModel; }

String PressureReader::getErrorMessage() { return errorMessage; }