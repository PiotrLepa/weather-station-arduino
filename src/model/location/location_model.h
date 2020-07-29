#ifndef LOCATION_MODEL_H
#define LOCATION_MODEL_H

struct LocationModel {
  float latitude;
  float longitude;

  LocationModel(float _latitude, float _longitudee)
      : latitude(_latitude), longitude(_longitudee) {}

  LocationModel() : latitude(-1), longitude(-1) {}
};

#endif