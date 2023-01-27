#include "pressure_reader.h"

PressureReader::PressureReader() : bme() {}

bool PressureReader::begin() {
  Wire1.begin(21, 22);
  return bme.begin(BME280_ADDRESS_ALTERNATE, &Wire1);
}

PressureModel PressureReader::read() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100;
  float humidity = bme.readHumidity();

  if (pressure == 0 || isnan(pressure) || isnan(temperature) || temperature < -100) {
    return PressureModel::error();
  }
  return PressureModel(temperature, pressure, humidity);
}