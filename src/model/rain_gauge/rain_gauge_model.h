#ifndef RAIN_GAUGE_MODEL_H
#define RAIN_GAUGE_MODEL_H

struct RainGaugeModel {
  float amountOfPrecipitation;
  bool hasError;

  RainGaugeModel(float _amountOfPrecipitation)
      : amountOfPrecipitation(_amountOfPrecipitation), hasError(false) {}

  RainGaugeModel() : amountOfPrecipitation(-1), hasError(true) {}

  RainGaugeModel static error() { return RainGaugeModel(); }
};

#endif