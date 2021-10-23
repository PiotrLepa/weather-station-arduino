#ifndef MAIN_H

#include <Arduino.h>
#include <Ticker.h>

#include "config/config.h"
#include "config/private_config.h"
#include "model/air_quality/air_quality_model.h"
#include "model/external_temperature/external_temperature_model.h"
#include "model/pressure/pressure_model.h"
#include "model/rain_gauge/rain_gauge_model.h"
#include "model/temperature/temperature_model.h"
#include "model/weather/weather_model.h"
#include "model/wind/wind_model.h"
#include "network/repository/weather_repository.h"
#include "network/rest_client/rest_client.h"
#include "network/wifi_client/wifi_client.h"
#include "sensor/air_quality_reader/air_quality_reader.h"
#include "sensor/external_temperature_reader/external_temperature_reader.h"
#include "sensor/pressure_reader/pressure_reader.h"
#include "sensor/rain_gauge/rain_gauge_callbacks.h"
#include "sensor/rain_gauge/rain_gauge_reader.h"
#include "sensor/temperature_reader/temperature_reader.h"
#include "sensor/wind_reader/wind_reader.h"
#include "storage/sd_card/sd_card_storage.h"
#include "utils/date_time/date_time.h"
#include "utils/json_coder/json_coder.h"

#define TEMPERATURE_SENSOR_PIN 32
#define EXTERNAL_TEMPERATURE_SENSOR_PIN 33
#define WIND_SENSOR_PIN 13
#define RAIN_GAUGE_SENSOR_PIN 4
#define PMS_MODE_CONTROL_PIN 14
#define WIFI_STATUS_PIN 2

#define SERVER_REQUEST_DELAY 300000 - PMS_WAKE_UP_MILLIS  // 5 minutes

void setup();
void loop();
void startSensors();
ConnectionResult connectToWifi();
void setWifiLed(bool isWifiEnabled);
void checkIfRainHasBeenDetected();
void collectWeatherData();
void scanAndSendWifiList();
void wakeUpSensors();
void sendWeatherDataToServer(TemperatureModel temperature, ExternalTemperatureModel externalTemperature,
                             PressureModel pressureModel, AirQualityModel airQuality, WindModel wind,
                             RainGaugeModel rainGauge);

#endif