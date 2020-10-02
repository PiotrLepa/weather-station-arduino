#include "json_coder.h"

String JsonCoder::encodeWeatherModel(WeatherModel model) {
  StaticJsonDocument<200> doc;

  doc["temperature"] = formatTemperature(model.temperature, model.pressure);
  doc["humidity"] =
      model.temperature.hasError ? NAN : model.temperature.humidity;
  doc["pressure"] = formatPressure(model.pressure);
  doc["pm1"] = model.airQuality.hasError ? NAN : model.airQuality.pm1;
  doc["pm25"] = model.airQuality.hasError ? NAN : model.airQuality.pm25;
  doc["pm10"] = model.airQuality.hasError ? NAN : model.airQuality.pm10;
  doc["windSpeedMax"] = model.wind.hasError ? NAN : model.wind.windSpeedMax;
  doc["windSpeedAvg"] = model.wind.hasError ? NAN : model.wind.windSpeedAvg;
  doc["rainGauge"] =
      model.rainGauge.hasError ? NAN : model.rainGauge.amountOfPrecipitation;

  String json;
  serializeJsonPretty(doc, json);
  return json;
}

String JsonCoder::encodeWifiNameList(std::vector<WifiNameModel> models) {
  size_t capacity = JSON_ARRAY_SIZE(models.size() * 10);
  DynamicJsonDocument doc(capacity);

  JsonArray array = doc.to<JsonArray>();

  for (int i = 0; i < models.size(); i++) {
    JsonObject nested = array.createNestedObject();
    nested["name"] = models[i].name;
  }

  String json;
  serializeJson(array, json);
  return json;
}

double JsonCoder::formatTemperature(TemperatureModel temp1,
                                    PressureModel temp2) {
  double resultTemp = 0;
  int tempReadCounter = 0;
  if (!temp1.hasError) {
    resultTemp += temp1.temperature;
    tempReadCounter++;
  }
  if (!temp2.hasError) {
    resultTemp += temp2.temperature;
    tempReadCounter++;
  }

  if (tempReadCounter == 0) {
    return NAN;
  } else {
    resultTemp /= tempReadCounter;

    // round to 2 decimal point
    int temp = round(resultTemp * 100);
    return temp / 100;
  }
}

double JsonCoder::formatPressure(PressureModel model) {
  if (!model.hasError) {
    return round(model.pressure / 100);
  } else {
    return NAN;
  }
}