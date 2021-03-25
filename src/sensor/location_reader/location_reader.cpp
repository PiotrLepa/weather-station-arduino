#include "location_reader.h"

LocationReader::LocationReader(HardwareSerial& serial, uint8_t _gpsRxPin, uint8_t _gpsTxPin)
    : gpsRxPin(_gpsRxPin), gpsTxPin(_gpsTxPin), gpsSerial(serial), readModel(-1, -1), errorMessage("No errors") {}

void LocationReader::begin() { gpsSerial.begin(9600, SERIAL_8N1, gpsTxPin, gpsRxPin); }

void LocationReader::update() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
}

bool LocationReader::read() {
  TinyGPSLocation location = gps.location;

  if (!location.isValid() || location.lat() < 1 || location.lng() < 1) {
    readModel = LocationModel::error();
    errorMessage = "Failed to read from GPS sensor!";
    return false;
  }

  readModel = LocationModel(location.lat(), location.lng());
  return true;
}

LocationModel LocationReader::getData() { return readModel; }

String LocationReader::getErrorMessage() { return errorMessage; }