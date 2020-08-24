#ifndef MAIN_H

#include <Arduino.h>
#include <Ticker.h>

#include "config/config.h"
#include "config/private_config.h"
#include "json_encoder/json_encoder.h"
#include "model/air_quality/air_quality_model.h"
#include "model/location/location_model.h"
#include "model/pressure/pressure_model.h"
#include "model/rain_gauge/rain_gauge_model.h"
#include "model/temperature/temperature_model.h"
#include "model/weather/weather_model.h"
#include "model/wind/wind_model.h"
#include "rest_client/rest_client.h"
#include "sensor/air_quality_reader/air_quality_reader.h"
#include "sensor/location_reader/location_reader.h"
#include "sensor/pressure_reader/pressure_reader.h"
#include "sensor/rain_gauge/rain_gauge_reader.h"
#include "sensor/temperature_reader/temperature_reader.h"
#include "sensor/wind_reader/wind_reader.h"
#include "weather_repository/weather_repository.h"

#define TEMPERATURE_SENSOR_PIN 32
#define WIND_SENSOR_PIN 23
#define RAIN_GAUGE_SENSOR_PIN 22
#define GPS_SENSOR_RX_PIN 26
#define GPS_SENSOR_TX_PIN 27

#define SERVER_REQUEST_DELAY 10000

void setup();
void loop();
void begin();
void startSensors();
void gatherWeatherData();
void sendWeatherDataToServer(TemperatureModel temperature,
                             PressureModel pressureModel,
                             AirQualityModel airQuality, WindModel wind,
                             RainGaugeModel rainGauge);

#endif