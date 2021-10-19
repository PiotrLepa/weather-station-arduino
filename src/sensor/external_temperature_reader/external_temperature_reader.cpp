#include "external_temperature_reader.h"

ExternalTemperatureReader::ExternalTemperatureReader(uint8_t pin) : readModel(-1), errorMessage("No errors") {
  OneWire oneWire(pin);
  sensor = DallasTemperature(&oneWire);
}

void ExternalTemperatureReader::begin() {}

bool ExternalTemperatureReader::read() {
  sensor.requestTemperatures();
  float temperature = sensor.getTempCByIndex(0);

  if (isnan(temperature) || temperature == -1) {
    readModel = ExternalTemperatureModel::error();
    errorMessage = "Failed to read from external temperature(DS18B20) sensor!";
    return false;
  }
  readModel = ExternalTemperatureModel(temperature);
  return true;
}

ExternalTemperatureModel ExternalTemperatureReader::getData() { return readModel; }

String ExternalTemperatureReader::getErrorMessage() { return errorMessage; }