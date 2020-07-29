#include "location_reader.h"

LocationReader::LocationReader(uint8_t rxPin, uint8_t txPin)
    : readModel(-1, -1), errorMessage("No errors") {
  // gpsSerial = Serial;
}

void LocationReader::begin() {
  // gpsSerial.begin(9600);
}

bool LocationReader::read() {
  // SoftwareSerial gpsSerial(D6, D7);
  // gpsSerial.begin(9600);

  // while (!gpsSerial.available()) {
  //   delay(500);
  //   Serial.print(".");
  // }

  // Serial.print("available: ");
  // Serial.println(gpsSerial.available());

  int data = Serial.read();
  Serial.println(data);
  gps.encode(data);
  TinyGPSLocation location = gps.location;

  if (!location.isValid()) {
    readModel = LocationModel(-1, -1);
    errorMessage = "Failed to read from GPS sensor!";
    return false;
  }
  readModel = LocationModel(location.lat(), location.lng());
  return true;
}

LocationModel LocationReader::getData() { return readModel; }

String LocationReader::getErrorMessage() { return errorMessage; }