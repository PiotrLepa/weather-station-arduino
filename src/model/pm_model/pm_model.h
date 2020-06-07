#ifndef PM_MODEL_H
#define PM_MODEL_H

struct PmModel {
  int pm1;
  int pm25;
  int pm10;

  PmModel(int _pm1, int _pm25, int _pm10)
      : pm1(_pm1), pm25(_pm25), pm10(_pm10) {}
};

#endif