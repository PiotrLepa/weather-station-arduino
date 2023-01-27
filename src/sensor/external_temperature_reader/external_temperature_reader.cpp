#include "external_temperature_reader.h"

ExternalTemperatureReader::ExternalTemperatureReader(OneWire* oneWire) : sensor(oneWire) {}

bool ExternalTemperatureReader::begin() {
  sensor.begin();
  return true;
}

ExternalTemperatureModel ExternalTemperatureReader::read() {
  sensor.requestTemperatures();
  sensor.requestTemperatures();
  float temperature = sensor.getTempCByIndex(0);

  if (isnan(temperature) || temperature == -1 || temperature == -127) {
    return ExternalTemperatureModel::error();
  }
  return ExternalTemperatureModel(temperature);
}