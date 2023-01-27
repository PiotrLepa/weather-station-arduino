#ifndef MAIN_H

#include <Arduino.h>
#include <Ticker.h>

#include "config/config.h"
#include "config/wifi_credentials.h"
#include "model/air_quality/air_quality_model.h"
#include "model/external_temperature/external_temperature_model.h"
#include "model/pressure/pressure_model.h"
#include "model/rain_gauge/rain_gauge_model.h"
#include "model/temperature/temperature_model.h"
#include "model/weather/weather_model.h"
#include "model/wind/wind_model.h"
#include "network/repository/weather_repository.h"
#include "network/repository/rest/rest_weather_repository.h"
#include "network/repository/firestore/firestore_weather_repository.h"
#include "network/client/rest/rest_client.h"
#include "network/client/firestore/firestore_client.h"
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
#include "utils/rounder/weather/weather_model_rounder.h"
#include "utils/json_coder/json_coder.h"
#include "utils/printer/weather_printer.h"
#include "utils/logger/logger.h"

#define EXTERNAL_TEMPERATURE_SENSOR_PIN 26
#define PMS_MODE_CONTROL_PIN 13
#define WIND_SENSOR_PIN 14
#define RAIN_GAUGE_SENSOR_PIN 33
#define WIFI_STATUS_PIN 2

// #define SERVER_REQUEST_DELAY 300000 - PMS_WAKE_UP_MILLIS  // 5 minutes
// #define SERVER_REQUEST_DELAY 180000 - PMS_WAKE_UP_MILLIS  // 3 minutes
#define SERVER_REQUEST_DELAY 90000 - PMS_WAKE_UP_MILLIS  // 1.5 minutes

//   pio device monitor --filter esp32_exception_decoder

#define FORMAT_SAVED_LOGS_ON_FAIL true

void setup();
void loop();
void startSensors();
ConnectionResult connectToWifi();
void setWifiLed(bool isWifiEnabled);
void collectWeatherData();
void scanAndSendWifiList();
void wakeUpSensors();
void sendWeatherDataToServer(ExternalTemperatureModel externalTemperature, PressureModel pressureModel,
                             AirQualityModel airQuality, WindModel wind, RainGaugeModel rainGauge);
String getCurrentTimestamp();

#endif