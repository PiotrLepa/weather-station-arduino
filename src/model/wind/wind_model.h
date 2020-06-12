#ifndef WIND_MODEL_H
#define WIND_MODEL_H

struct WindModel {
  float windSpeedMax;
  float windSpeedMin;
  float windSpeedAvg;

  WindModel(float _windSpeedMax, float _windSpeedMin, float _windSpeedAvg)
      : windSpeedMax(_windSpeedMax),
        windSpeedMin(_windSpeedMin),
        windSpeedAvg(_windSpeedAvg) {}
};

#endif