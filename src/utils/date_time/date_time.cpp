#include "date_time.h"

bool DateTime::isInitialized = false;

DateTime::DateTime(int _year, int _month, int _day, int _hour, int _minute, int _second)
    : year(_year), month(_month), day(_day), hour(_hour), minute(_minute), second(_second) {}

void DateTime::begin() {
  configTime(GMT_OFFSET_SECONDS, DAY_LIGHT_OFFSET_SECONDS, NTP_SERVER);
  isInitialized = true;
}

DateTime DateTime::now() {
  struct tm timeinfo;
  if (!isInitialized || !getLocalTime(&timeinfo)) {
    return DateTime(-1, -1, -1, -1, -1, -1);
  }

  return DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

int DateTime::getYear() { return year; }
int DateTime::getMonth() { return month; }
int DateTime::getDay() { return day; }
int DateTime::getHour() { return hour; }
int DateTime::getMinute() { return minute; }
int DateTime::getSecond() { return second; }

String DateTime::getFormattedDateTime() {
  if (year == -1) return "";
  return String(year) + "-" + formatNumber(month) + "-" + formatNumber(day) + "T" + formatNumber(hour) + ":" + formatNumber(minute) + ":" +
         formatNumber(second) + "Z";
}

DateTime DateTime::atMidnight() { return DateTime(year, month, day, 0, 0, 0); }

String DateTime::formatNumber(int number) {
  if (number >= 10) {
    return String(number);
  } else {
    return "0" + String(number);
  }
}

bool operator==(const DateTime& object1, const DateTime& object2) {
  return object1.year == object2.year && object1.month == object2.month && object1.day == object2.day && object1.hour == object2.hour &&
         object1.minute == object2.minute && object1.second == object2.second;
}
bool operator!=(const DateTime& object1, const DateTime& object2) { return !(object1 == object2); }