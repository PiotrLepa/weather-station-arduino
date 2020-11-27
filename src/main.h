#ifndef MAIN_H

#include <Arduino.h>
#include <Ticker.h>

#include "config/config.h"
#include "model/air_quality/air_quality_model.h"
#include "model/location/location_model.h"
#include "model/pressure/pressure_model.h"
#include "model/rain_gauge/rain_gauge_model.h"
#include "model/temperature/temperature_model.h"
#include "model/weather/weather_model.h"
#include "model/wind/wind_model.h"
#include "network/bluetooth/ble_callbacks.h"
#include "network/bluetooth/ble_manager.h"
#include "network/repository/weather_repository.h"
#include "network/rest_client/rest_client.h"
#include "network/wifi_client/wifi_client.h"
#include "sensor/air_quality_reader/air_quality_reader.h"
#include "sensor/location_reader/location_reader.h"
#include "sensor/pressure_reader/pressure_reader.h"
#include "sensor/rain_gauge/rain_gauge_callbacks.h"
#include "sensor/rain_gauge/rain_gauge_reader.h"
#include "sensor/temperature_reader/temperature_reader.h"
#include "sensor/wind_reader/wind_reader.h"
#include "storage/eeprom/eeprom_storage.h"
#include "storage/sd_card/sd_card_storage.h"
#include "utils/date_time/date_time.h"
#include "utils/json_coder/json_coder.h"

#define TEMPERATURE_SENSOR_PIN 32
#define WIND_SENSOR_PIN 13
#define RAIN_GAUGE_SENSOR_PIN 4
#define GPS_SENSOR_TX_PIN 12
#define GPS_SENSOR_RX_PIN 14

#define SERVER_REQUEST_DELAY 300000  // 5 minutes
#define START_SCAN_WIFI_DELAY 2000

void setup();
void loop();
void startSensors();
void connectToWifiIfCredentialsAreSaved();
ConnectionResult connectToWifiAndSetupOnSuccess(String credentialsJson, bool saveCredentials);
void checkIfRainHasBeenDetected();
void gatherWeatherData();
void scanAndSendWifiList();
void sendWeatherDataToServer(TemperatureModel temperature, PressureModel pressureModel, AirQualityModel airQuality,
                             WindModel wind, RainGaugeModel rainGauge, LocationModel location);

#endif