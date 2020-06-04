#include "json_encoder.h"

String JsonEncoder::encodeWeatherModel(WeatherModel model) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = model.temperature;
  String json;
  serializeJsonPretty(doc, json);
  return json;
}