#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <Arduino.h>
#include <time.h>

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SECONDS 0
#define DAY_LIGHT_OFFSET_SECONDS 0

class DateTime {
 public:
  static void begin();
  static DateTime now();

  long getSecondsFromEpoch();
  String getFormattedDate();

 private:
  static bool isInitialized;

  long secondsFromEpoch;
  String formattedDate;

  DateTime(long _secondsFromEpoch, String _formattedDate);
  static String formatNumber(int number);
};

#endif