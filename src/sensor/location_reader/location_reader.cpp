#include "location_reader.h"

LocationReader::LocationReader(uint8_t rxPin, uint8_t txPin) : readModel(-1, -1), errorMessage("No errors") {
  gpsSerial = new SoftwareSerial(rxPin, txPin);
}

void LocationReader::begin() { gpsSerial->begin(9600); }

void LocationReader::update() {
  while (gpsSerial->available()) {
    gps.encode(gpsSerial->read());
  }
}

bool LocationReader::read() {
  TinyGPSLocation location = gps.location;

  if (!location.isValid()) {
    readModel = LocationModel::error();
    errorMessage = "Failed to read from GPS sensor!";
    return false;
  }
  readModel = LocationModel(location.lat(), location.lng());
  return true;
}

LocationModel LocationReader::getData() { return readModel; }

String LocationReader::getErrorMessage() { return errorMessage; }