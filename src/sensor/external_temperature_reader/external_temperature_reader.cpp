#include "external_temperature_reader.h"

ExternalTemperatureReader::ExternalTemperatureReader(OneWire* oneWire) : sensor(oneWire), readModel(-1), errorMessage("") {}

bool ExternalTemperatureReader::begin() {
  sensor.begin();
  return true;
}

bool ExternalTemperatureReader::read() {
  sensor.requestTemperatures();
  sensor.requestTemperatures();
  float temperature = sensor.getTempCByIndex(0);

  if (isnan(temperature) || temperature == -1 || temperature == -127) {
    readModel = ExternalTemperatureModel::error();
    errorMessage = "Failed to read from external temperature(DS18B20) sensor!";
    return false;
  }
  errorMessage = "";
  readModel = ExternalTemperatureModel(temperature);
  return true;
}

ExternalTemperatureModel ExternalTemperatureReader::getData() { return readModel; }

String ExternalTemperatureReader::getErrorMessage() { return errorMessage; }