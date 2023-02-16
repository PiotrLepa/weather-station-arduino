#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <Arduino.h>
#include <time.h>

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SECONDS 0
#define DAY_LIGHT_OFFSET_SECONDS 0

class DateTime {
 public:
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;

  static void begin();
  static DateTime now();

  int getYear();
  int getMonth();
  int getDay();
  int getHour();
  int getMinute();
  int getSecond();
  String getFormattedDateTime();
  DateTime atMidnight();

  friend bool operator==(const DateTime& object1, const DateTime& object2);
  friend bool operator!=(const DateTime& object1, const DateTime& object2);

 private:
  static bool isInitialized;

  DateTime(int _year, int _month, int _day, int _hour, int _minute, int _second);
  static String formatNumber(int number);
};

bool operator==(const DateTime& object1, const DateTime& object2);
bool operator!=(const DateTime& object1, const DateTime& object2);

#endif