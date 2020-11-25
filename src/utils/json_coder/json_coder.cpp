#include "json_coder.h"

String JsonCoder::encodeWeatherModel(WeatherModel model) {
  StaticJsonDocument<256> doc;

  doc["temperature"] = formatTemperature(model.temperature, model.pressure);
  doc["humidity"] = model.temperature.hasError ? NAN : formatToOneDecimalPoint(model.temperature.humidity);
  doc["pressure"] = model.pressure.hasError ? NAN : formatToOneDecimalPoint(model.pressure.pressure);
  doc["pm1"] = model.airQuality.hasError ? NAN : model.airQuality.pm1;
  doc["pm25"] = model.airQuality.hasError ? NAN : model.airQuality.pm25;
  doc["pm10"] = model.airQuality.hasError ? NAN : model.airQuality.pm10;
  doc["windSpeedMax"] = model.wind.hasError ? NAN : formatToOneDecimalPoint(model.wind.windSpeedMax);
  doc["windSpeedAvg"] = model.wind.hasError ? NAN : formatToOneDecimalPoint(model.wind.windSpeedAvg);
  doc["rainGauge"] = model.rainGauge.hasError ? NAN : formatToOneDecimalPoint(model.rainGauge.amountOfPrecipitation);
  doc["latitude"] = model.location.hasError ? NAN : model.location.latitude;
  doc["longitude"] = model.location.hasError ? NAN : model.location.longitude;

  printJson(doc);
  String json;
  serializeJson(doc, json);
  return json;
}

String JsonCoder::encodeCachedWeatherModel(CachedWeatherModel model) {
  StaticJsonDocument<256> doc;

  JsonObject weather = doc.createNestedObject("weather");
  WeatherModel weatherModel = model.weather;
  weather["temperature"] = formatTemperature(weatherModel.temperature, weatherModel.pressure);
  weather["humidity"] =
      weatherModel.temperature.hasError ? NAN : formatToOneDecimalPoint(weatherModel.temperature.humidity);
  weather["pressure"] = weatherModel.pressure.hasError ? NAN : formatToOneDecimalPoint(weatherModel.pressure.pressure);
  weather["pm1"] = weatherModel.airQuality.hasError ? NAN : weatherModel.airQuality.pm1;
  weather["pm25"] = weatherModel.airQuality.hasError ? NAN : weatherModel.airQuality.pm25;
  weather["pm10"] = weatherModel.airQuality.hasError ? NAN : weatherModel.airQuality.pm10;
  weather["windSpeedMax"] = weatherModel.wind.hasError ? NAN : formatToOneDecimalPoint(weatherModel.wind.windSpeedMax);
  weather["windSpeedAvg"] = weatherModel.wind.hasError ? NAN : formatToOneDecimalPoint(weatherModel.wind.windSpeedAvg);
  weather["rainGauge"] =
      weatherModel.rainGauge.hasError ? NAN : formatToOneDecimalPoint(weatherModel.rainGauge.amountOfPrecipitation);
  doc["latitude"] = weatherModel.location.hasError ? NAN : weatherModel.location.latitude;
  doc["longitude"] = weatherModel.location.hasError ? NAN : weatherModel.location.longitude;

  doc["timestamp"] = model.timestamp;
  printJson(doc);

  String json;
  serializeJson(doc, json);
  return json;
}

String JsonCoder::encodeWifiNameList(std::vector<WifiModel> models) {
  size_t capacity = JSON_ARRAY_SIZE(models.size() * 10);
  DynamicJsonDocument doc(capacity);

  JsonArray array = doc.to<JsonArray>();

  for (int i = 0; i < models.size(); i++) {
    JsonObject nested = array.createNestedObject();
    nested["name"] = models[i].name;
    nested["encryption"] = encryptionToString(models[i].encryption);
    nested["rssi"] = models[i].rssi;
  }

  printJson(array);

  String json;
  serializeJson(array, json);
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

WifiCredentialsModel JsonCoder::decodeWifiCredentials(String json) {
  StaticJsonDocument<128> doc;
  deserializeJson(doc, json);

  printJson(doc);

  return WifiCredentialsModel(doc["name"], doc["password"]);
}

double JsonCoder::formatToOneDecimalPoint(double value) { return round(value * 10) / 10; }

double JsonCoder::formatTemperature(TemperatureModel temp1, PressureModel temp2) {
  if (!temp1.hasError) {
    return formatToOneDecimalPoint(temp1.temperature);
  } else if (!temp2.hasError) {
    return formatToOneDecimalPoint(temp2.temperature);
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