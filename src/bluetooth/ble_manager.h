#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <Adafruit_Sensor.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "../json_coder/json_coder.h"
#include "../model/wifi/wifi_model.h"
#include "ble_callbacks.h"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define BLE_NAME "ESP-32 WeatherStation"

class BleManager {
 public:
  BleManager(JsonCoder _jsonCoder);

  void begin(BleCallbacks* _callbacks);
  void sendAvailableWifiList(std::vector<WifiModel> models);

 private:
  JsonCoder jsonCoder;
  BLECharacteristic* pCharacteristic;
};

#endif