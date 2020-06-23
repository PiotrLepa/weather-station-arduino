#include "pressure_reader.h"

PressureReader::PressureReader()
    : bme(), readModel(-1, -1), errorMessage("No errors") {}

void PressureReader::begin() {
  if (!bme.begin(BME280_ADDRESS_ALTERNATE)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1)
      ;
  }
}

bool PressureReader::read() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure();

  if (isnan(temperature) || isnan(pressure)) {
    errorMessage = "Failed to read from DHT sensor!";
    return false;
  }
  readModel = PressureModel(temperature, pressure);
  return true;
}

PressureModel PressureReader::getData() { return readModel; }

String PressureReader::getErrorMessage() { return errorMessage; }