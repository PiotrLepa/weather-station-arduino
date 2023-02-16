#include "json_coder.h"

String JsonCoder::encodeWeather(WeatherModel model) {
  StaticJsonDocument<256> doc;

  doc["temperature"] = model.externalTemperature.hasError ? NAN : model.externalTemperature.temperature;
  doc["humidity"] = model.pressure.hasError ? NAN : model.pressure.humidity;
  doc["pressure"] = model.pressure.hasError ? NAN : model.pressure.pressure;
  doc["pm1"] = model.airQuality.hasError ? NAN : model.airQuality.pm1;
  doc["pm25"] = model.airQuality.hasError ? NAN : model.airQuality.pm25;
  doc["pm10"] = model.airQuality.hasError ? NAN : model.airQuality.pm10;
  doc["windSpeedMax"] = model.wind.hasError ? NAN : model.wind.windSpeedMax;
  doc["windSpeedAvg"] = model.wind.hasError ? NAN : model.wind.windSpeedAvg;
  doc["precipitation"] = model.rainGauge.hasError ? NAN : model.rainGauge.precipitation;
  doc["timestamp"] = model.timestamp.getFormattedDateTime();

  printJson(doc);
  String json;
  serializeJson(doc, json);
  return json;
}

String JsonCoder::encodeWeathersList(std::vector<String> jsonModels) {
  String resultJson = "[";
  for (String json : jsonModels) {
    resultJson += json + ",";
  }
  resultJson.remove(resultJson.length() - 1);  // remove last ','
  resultJson += "]";

  return resultJson;
}

void JsonCoder::printJson(JsonDocument &source) {
  String json;
  serializeJsonPretty(source, json);
  Serial.print("Json: ");
  Serial.println(json);
}