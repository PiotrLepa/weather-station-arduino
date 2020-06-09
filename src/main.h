
#ifndef MAIN_H

#include <Arduino.h>

#include "config/config.h"
#include "config/private_config.h"
#include "controller/dht_controller/dht_controller.h"
#include "controller/pm_controller/pm_controller.h"
#include "json_encoder/json_encoder.h"
#include "model/air_quality/air_quality_model.h"
#include "model/temperature/temperature_model.h"
#include "model/weather/weather_model.h"
#include "rest_client/rest_client.h"
#include "weather_repository/weather_repository.h"

#define DHT_PIN D1

void setup();
void loop();
void printTemperature(TemperatureModel model);
void printAirQuality(AirQualityModel model);

#endif