#include "json_encoder.h"

String JsonEncoder::encodeWeatherModel(WeatherModel model) {
  StaticJsonDocument<200> doc;

  doc["temperature"] = formatTemperature(model.temperature.temperature,
                                         model.pressure.temperature);
  doc["humidity"] = model.temperature.humidity;
  doc["pressure"] = formatPressure(model.pressure.pressure);
  doc["pm1"] = model.airQuality.pm1;
  doc["pm25"] = model.airQuality.pm25;
  doc["pm10"] = model.airQuality.pm10;
  doc["windSpeedMax"] = model.wind.windSpeedMax;
  doc["windSpeedAvg"] = model.wind.windSpeedAvg;
  doc["rainGauge"] = model.rainGauge.amountOfPrecipitation;

  String json;
  serializeJsonPretty(doc, json);
  return json;
}

double JsonEncoder::formatTemperature(double temp1, double temp2) {
  double resultTemp = 0;
  int tempReadCounter = 0;
  if (temp1 != -1) {
    resultTemp += temp1;
    tempReadCounter++;
  }
  if (temp2 != -1) {
    resultTemp += temp2;
    tempReadCounter++;
  }

  if (tempReadCounter == 0) {
    resultTemp = -1;
  } else {
    resultTemp /= tempReadCounter;
    resultTemp = round(resultTemp * 100) / 100;  // round to 2 decimal point
  }

  return resultTemp;
}

double JsonEncoder::formatPressure(double pressure) {
  if (pressure != -1) {
    return round(pressure / 100);
  } else {
    return -1;
  }
}