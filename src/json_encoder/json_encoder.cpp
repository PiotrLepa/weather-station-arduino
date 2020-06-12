#include "json_encoder.h"

String JsonEncoder::encodeWeatherModel(WeatherModel model) {
  StaticJsonDocument<200> doc;

  doc["temperature"] = model.temperature.temperature;
  doc["humidity"] = model.temperature.humidity;
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