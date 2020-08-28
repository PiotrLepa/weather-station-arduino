#ifndef WIND_MODEL_H
#define WIND_MODEL_H

struct WindModel {
  float windSpeedMax;
  float windSpeedAvg;
  bool hasError;

  WindModel(float _windSpeedMax, float _windSpeedAvg)
      : windSpeedMax(_windSpeedMax),
        windSpeedAvg(_windSpeedAvg),
        hasError(false) {}

  WindModel() : windSpeedMax(-1), windSpeedAvg(-1), hasError(true) {}

  WindModel static error() { return WindModel(); }
};

#endif