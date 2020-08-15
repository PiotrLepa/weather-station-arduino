#ifndef LOCATION_READER_H
#define LOCATION_READER_H
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include "../../model/location/location_model.h"
#include "../sensor_reader.h"

class LocationReader : public SensorReader<LocationModel> {
 public:
  LocationReader(uint8_t rxPin, uint8_t txPin);

  void begin() override;
  bool read() override;
  LocationModel getData() override;
  String getErrorMessage() override;
  void update();

 private:
  TinyGPSPlus gps;
  SoftwareSerial *gpsSerial;
  LocationModel readModel;
  String errorMessage;
};

#endif