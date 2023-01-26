#include "date_time.h"

bool DateTime::isInitialized = false;

DateTime::DateTime(long _secondsFromEpoch, String _formattedDate)
    : secondsFromEpoch(_secondsFromEpoch), formattedDate(_formattedDate) {}

void DateTime::begin() {
  configTime(GMT_OFFSET_SECONDS, DAY_LIGHT_OFFSET_SECONDS, NTP_SERVER);
  isInitialized = true;
}

DateTime DateTime::now() {
  struct tm timeinfo;
  if (!isInitialized || !getLocalTime(&timeinfo)) {
    return DateTime(-1, "");
  }

  String formatted = String(timeinfo.tm_year + 1900) + "-" + formatNumber(timeinfo.tm_mon + 1) + "-" +
                     formatNumber(timeinfo.tm_mday) + "T" + formatNumber(timeinfo.tm_hour) + ":" +
                     formatNumber(timeinfo.tm_min) + ":" + formatNumber(timeinfo.tm_sec) + ".000" + "Z";

  return DateTime(time(nullptr), formatted);
}

long DateTime::getSecondsFromEpoch() { return secondsFromEpoch; }

String DateTime::getFormattedDate() { return formattedDate; }

String DateTime::formatNumber(int number) {
  if (number >= 10) {
    return String(number);
  } else {
    return "0" + String(number);
  }
}