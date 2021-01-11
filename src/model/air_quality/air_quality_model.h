#ifndef AIR_QUALITY_MODEL_H
#define AIR_QUALITY_MODEL_H

struct AirQualityModel {
  int pm1;
  int pm25;
  int pm10;
  bool hasError;

  AirQualityModel(int _pm1, int _pm25, int _pm10) : pm1(_pm1), pm25(_pm25), pm10(_pm10), hasError(false) {}

  AirQualityModel() : pm1(-1), pm25(-1), pm10(-1), hasError(true) {}

  AirQualityModel static error() { return AirQualityModel(); }
};

#endif