#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <Adafruit_Sensor.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <list>

#include "../json_coder/json_coder.h"
#include "../model/wifi/wifi_model.h"
#include "ble_callbacks.h"

#define BLE_NAME "ESP-32 WeatherStation"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define WIFI_SCAN_CHARACTERISTIC "53ce635f-255d-4cdb-9ece-dc8ba92180aa"
#define WIFI_LIST_CHARACTERISTIC "db7a9839-79a5-455f-a213-736f25691050"

#define PART_SIZE 500

class BleManager {
  enum MessageType { START, END, PART };

 public:
  BleCallbacks* callbacks;
  std::list<String> partsToSend;
  String nextPart = "END";

  BleManager(JsonCoder _jsonCoder);

  void begin(BleCallbacks* _callbacks);
  void sendWifiList(std::vector<WifiModel> models);

 private:
  JsonCoder jsonCoder;
  BLECharacteristic* wifiListCharacteristic;
};

#endif