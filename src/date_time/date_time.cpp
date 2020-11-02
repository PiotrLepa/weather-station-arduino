#include "date_time.h"

void DateTime::begin() {
  configTime(GMT_OFFSET_SECONDS, DAY_LIGHT_OFFSET_SECONDS, NTP_SERVER);
}

String DateTime::now() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "";
  }

  return String(timeinfo.tm_year + 1900) + "-" + formatNumber(timeinfo.tm_mon) +
         "-" + formatNumber(timeinfo.tm_mday) + "T" +
         formatNumber(timeinfo.tm_hour) + ":" + formatNumber(timeinfo.tm_min) +
         ":" + formatNumber(timeinfo.tm_sec);
}

String DateTime::formatNumber(int number) {
  if (number >= 10) {
    return String(number);
  } else {
    return "0" + String(number);
  }
}