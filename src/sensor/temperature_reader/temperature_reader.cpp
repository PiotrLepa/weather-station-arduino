#include "temperature_reader.h"

TemperatureReader::TemperatureReader(uint8_t pin) : dht(pin, DHT22) {}

bool TemperatureReader::begin() {
  dht.begin();
  return true;
}

TemperatureModel TemperatureReader::read() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    return TemperatureModel::error();
  }
  return TemperatureModel(temperature, humidity);
}