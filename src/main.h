#ifndef MAIN_H

#include <Arduino.h>

#include "config/config.h"
#include "config/private_config.h"
#include "json_encoder/json_encoder.h"
#include "model/air_quality/air_quality_model.h"
#include "model/temperature/temperature_model.h"
#include "model/weather/weather_model.h"
#include "model/wind/wind_model.h"
#include "rest_client/rest_client.h"
#include "sensor/air_quality_reader/air_quality_reader.h"
#include "sensor/temperature_reader/temperature_reader.h"
#include "sensor/wind_reader/wind_reader.h"
#include "weather_repository/weather_repository.h"

#define DHT_PIN D1
#define WIND_SENSOR_PIN D2

void setup();
void loop();
void printTemperature(TemperatureModel model);
void printAirQuality(AirQualityModel model);

#endif