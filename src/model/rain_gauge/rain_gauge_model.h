#ifndef RAIN_GAUGE_MODEL_H
#define RAIN_GAUGE_MODEL_H

struct RainGaugeModel {
  float amountOfPrecipitation;

  RainGaugeModel(float _amountOfPrecipitation)
      : amountOfPrecipitation(_amountOfPrecipitation) {}

  RainGaugeModel() : amountOfPrecipitation(-1) {}
};

#endif