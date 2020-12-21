#ifndef LOCATION_READER_H
#define LOCATION_READER_H

#include <HardwareSerial.h>
#include <TinyGPS++.h>

#include "../../model/location/location_model.h"
#include "../sensor_reader.h"

class LocationReader : public SensorReader<LocationModel> {
 public:
  LocationReader(HardwareSerial& serial, uint8_t _gpsRxPin, uint8_t _gpsTxPin);

  void begin() override;
  bool read() override;
  LocationModel getData() override;
  String getErrorMessage() override;
  void update();

 private:
  uint8_t gpsRxPin;
  uint8_t gpsTxPin;
  HardwareSerial gpsSerial;
  TinyGPSPlus gps;
  LocationModel readModel;
  String errorMessage;
};

#endif