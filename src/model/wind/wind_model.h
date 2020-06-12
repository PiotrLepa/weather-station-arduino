#ifndef WIND_MODEL_H
#define WIND_MODEL_H

struct WindModel {
  float windSpeedMax;
  float windSpeedAvg;

  WindModel(float _windSpeedMax, float _windSpeedAvg)
      : windSpeedMax(_windSpeedMax), windSpeedAvg(_windSpeedAvg) {}

  WindModel() : windSpeedMax(-1), windSpeedAvg(-1) {}
};

#endif