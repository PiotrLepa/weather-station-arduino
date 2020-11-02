#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <Arduino.h>
#include <time.h>

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SECONDS 0
#define DAY_LIGHT_OFFSET_SECONDS 3600

class DateTime {
 public:
  void begin();
  String now();

 private:
  String formatNumber(int number);
};

#endif