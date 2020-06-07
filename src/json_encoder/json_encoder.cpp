#include "json_encoder.h"

String JsonEncoder::encodeWeatherModel(WeatherModel model) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = model.temperature.temperature;
  doc["pm1"] = model.airQuality.pm1;
  doc["pm25"] = model.airQuality.pm25;
  doc["pm10"] = model.airQuality.pm10;
  String json;
  serializeJsonPretty(doc, json);
  return json;
}