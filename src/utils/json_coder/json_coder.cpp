#include "json_coder.h"

String JsonCoder::encodeWeatherModel(WeatherModel model) {
  StaticJsonDocument<256> doc;

  doc["temperature"] = formatTemperature(model.externalTemperature);
  doc["humidity"] = formatHumidity(model.pressure);
  doc["pressure"] = model.pressure.hasError ? NAN : formatToOneDecimalPoint(model.pressure.pressure);
  doc["pm1"] = model.airQuality.hasError ? NAN : model.airQuality.pm1;
  doc["pm25"] = model.airQuality.hasError ? NAN : model.airQuality.pm25;
  doc["pm10"] = model.airQuality.hasError ? NAN : model.airQuality.pm10;
  doc["windSpeedMax"] = model.wind.hasError ? NAN : formatToOneDecimalPoint(model.wind.windSpeedMax);
  doc["windSpeedAvg"] = model.wind.hasError ? NAN : formatToOneDecimalPoint(model.wind.windSpeedAvg);
  doc["rainGauge"] = model.rainGauge.hasError ? NAN : formatToOneDecimalPoint(model.rainGauge.amountOfPrecipitation);;
  doc["latitude"] = NAN;
  doc["longitude"] = NAN;

  printJson(doc);
  String json;
  serializeJson(doc, json);
  return json;
}

String JsonCoder::encodeCachedWeatherModel(CachedWeatherModel model) {
  StaticJsonDocument<256> doc;

  JsonObject weather = doc.createNestedObject("weather");
  WeatherModel weatherModel = model.weather;
  weather["temperature"] = formatTemperature(weatherModel.externalTemperature);
  weather["humidity"] = formatHumidity(weatherModel.pressure);
  weather["pressure"] = weatherModel.pressure.hasError ? NAN : formatToOneDecimalPoint(weatherModel.pressure.pressure);
  weather["pm1"] = weatherModel.airQuality.hasError ? NAN : weatherModel.airQuality.pm1;
  weather["pm25"] = weatherModel.airQuality.hasError ? NAN : weatherModel.airQuality.pm25;
  weather["pm10"] = weatherModel.airQuality.hasError ? NAN : weatherModel.airQuality.pm10;
  weather["windSpeedMax"] = weatherModel.wind.hasError ? NAN : formatToOneDecimalPoint(weatherModel.wind.windSpeedMax);
  weather["windSpeedAvg"] = weatherModel.wind.hasError ? NAN : formatToOneDecimalPoint(weatherModel.wind.windSpeedAvg);
  weather["rainGauge"] = weatherModel.rainGauge.hasError ? NAN : formatToOneDecimalPoint(weatherModel.rainGauge.amountOfPrecipitation);
  weather["latitude"] = NAN;
  weather["longitude"] = NAN;

  doc["timestamp"] = model.timestamp;
  printJson(doc);

  String json;
  serializeJson(doc, json);
  return json;
}

String JsonCoder::encodeCachedWeathersList(std::vector<String> jsonModels) {
  String resultJson = "[";
  for (String json : jsonModels) {
    resultJson += json + ",";
  }
  resultJson.remove(resultJson.length() - 1);  // remove last ','
  resultJson += "]";

  return resultJson;
}

double JsonCoder::formatToOneDecimalPoint(double value) { return round(value * 10) / 10; }

double JsonCoder::formatTemperature(ExternalTemperatureModel model) {
  if (!model.hasError) {
    return formatToOneDecimalPoint(model.temperature);
  } else {
    return NAN;
  }
}

double JsonCoder::formatHumidity(PressureModel model) {
  if (!model.hasError) {
    return formatToOneDecimalPoint(model.humidity);
  } else {
    return NAN;
  }
}

void JsonCoder::printJson(JsonDocument &source) {
  String json;
  serializeJsonPretty(source, json);
  Serial.print("Json: ");
  Serial.println(json);
}

void JsonCoder::printJson(JsonArray &source) {
  String json;
  serializeJsonPretty(source, json);
  Serial.print("Json: ");
  Serial.println(json);
}