#include "pressure_reader.h"

PressureReader::PressureReader() : bme(), readModel(-1, -1, -1), errorMessage("") {}

bool PressureReader::begin() {
  Wire1.begin(21, 22);
  return bme.begin(BME280_ADDRESS_ALTERNATE, &Wire1);
}

bool PressureReader::read() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100;
  float humidity = bme.readHumidity();

  if (pressure == 0 || isnan(pressure) || isnan(temperature) || temperature < -100) {
    readModel = PressureModel::error();
    errorMessage = "Failed to read from BME sensor!";
    return false;
  }
  errorMessage = "";
  readModel = PressureModel(temperature, pressure, humidity);
  return true;
}

PressureModel PressureReader::getData() { return readModel; }

String PressureReader::getErrorMessage() { return errorMessage; }