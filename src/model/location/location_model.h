#ifndef LOCATION_MODEL_H
#define LOCATION_MODEL_H

struct LocationModel {
  float latitude;
  float longitude;
  bool hasError;

  LocationModel(float _latitude, float _longitudee) : latitude(_latitude), longitude(_longitudee), hasError(false) {}

  LocationModel() : latitude(-1), longitude(-1), hasError(true) {}

  LocationModel static error() { return LocationModel(); }
};

#endif