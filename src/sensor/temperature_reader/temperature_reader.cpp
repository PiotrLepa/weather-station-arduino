#include "temperature_reader.h"

TemperatureReader::TemperatureReader(uint8_t pin)
    : dht(pin, DHT22), readModel(-1, -1), errorMessage("No errors") {}

void TemperatureReader::begin() { dht.begin(); }

bool TemperatureReader::read() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    readModel = TemperatureModel::error();
    errorMessage = "Failed to read from DHT sensor!";
    return false;
  }
  readModel = TemperatureModel(temperature, humidity);
  return true;
}

TemperatureModel TemperatureReader::getData() { return readModel; }

String TemperatureReader::getErrorMessage() { return errorMessage; }