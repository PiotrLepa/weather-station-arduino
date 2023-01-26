#ifndef RAIN_GAUGE_MODEL_H
#define RAIN_GAUGE_MODEL_H

struct RainGaugeModel {
  float precipitation;
  bool hasError;

  RainGaugeModel(float _amountOfPrecipitation) : precipitation(_amountOfPrecipitation), hasError(false) {}

  RainGaugeModel() : precipitation(-1), hasError(true) {}

  RainGaugeModel static error() { return RainGaugeModel(); }
};

#endif