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
#include "../wifi_client/wifi_client.h"
#include "ble_callbacks.h"

#define BLE_NAME "ESP-32 WeatherStation"
#define SERVICE_UUID "a386aa2b-ce06-460c-bd02-9743997288b2"
#define SCAN_WIFI_CHARACTERISTIC "53ce635f-255d-4cdb-9ece-dc8ba92180aa"
#define WIFI_LIST_CHARACTERISTIC "db7a9839-79a5-455f-a213-736f25691050"
#define CONNECT_TO_WIFI_RESULT_CHARACTERISTIC \
  "e5b9a6f3-2d49-447b-a924-b2d116ca8e3f"

#define PART_SIZE 500

enum MessageType { START, END, PART };

class BleManager {
 public:
  JsonCoder jsonCoder;
  BleCallbacks* callbacks;
  std::list<String> partsToSend;
  MessageType nextMessageType;

  BleManager(JsonCoder _jsonCoder);

  void begin(BleCallbacks* _callbacks);
  void sendWifiList(std::vector<WifiModel> models);
  void sendConnectToWifiResult(ConnectionResult status);

 private:
  BLECharacteristic* wifiListCharacteristic;
  BLECharacteristic* connectToWifiResultCharacteristic;

  void setupScanWifiCharacteristic(BLEService* service);
  void setupWifiListCharacteristic(BLEService* service);
  void setupConnectToWifiResultCharacteristic(BLEService* service);
};

#endif