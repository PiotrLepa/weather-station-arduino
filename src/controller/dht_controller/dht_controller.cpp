#include "dht_controller.h"

DhtController::DhtController(uint8_t pin)
    : dht(pin, DHT22), readModel(-1, -1), errorMessage("No errors") {}

void DhtController::begin() { dht.begin(); }

bool DhtController::read() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    errorMessage = "Failed to read from DHT sensor!";
    return false;
  }
  readModel = TemperatureModel(temperature, humidity);
  return true;
}

TemperatureModel DhtController::getData() { return readModel; }

String DhtController::getErrorMessage() { return errorMessage; }