#include "ble_manager.h"

BleManager *instance = NULL;

// class ServerCallbacks : public BLEServerCallbacks {
//   void onConnect(BLEServer *pServer) {
//   instance->callbacks->scanAvailablesWifi(); };

//   void onDisconnect(BLEServer *pServer) {}
// };

class ScanWifiCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    instance->callbacks->scanAvailablesWifi();
  }
};

class WifiListCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    instance->callbacks->connectToWifi();
  }

  void onRead(BLECharacteristic *pCharacteristic) {
    if (instance->nextPart == "START") {
    } else if (instance->nextPart == "END") {
    } else {
    }
    pCharacteristic->setValue(instance->partsToSend.front().c_str());
    instance->partsToSend.pop_front();

    if (!instance->partsToSend.empty()) {
      pCharacteristic->notify();
    }
  }
};

BleManager::BleManager(JsonCoder _jsonCoder) : jsonCoder(_jsonCoder) {
  instance = this;
}

void BleManager::begin(BleCallbacks *_callbacks) {
  callbacks = _callbacks;

  BLEDevice::init(BLE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  // pServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *startScanCharacteristic = pService->createCharacteristic(
      WIFI_SCAN_CHARACTERISTIC, BLECharacteristic::PROPERTY_READ);
  startScanCharacteristic->setCallbacks(new ScanWifiCallbacks());
  startScanCharacteristic->addDescriptor(new BLE2902());
  startScanCharacteristic->setValue("OK");

  wifiListCharacteristic = pService->createCharacteristic(
      WIFI_LIST_CHARACTERISTIC, BLECharacteristic::PROPERTY_NOTIFY |
                                    BLECharacteristic::PROPERTY_READ |
                                    BLECharacteristic::PROPERTY_WRITE);
  wifiListCharacteristic->setCallbacks(new WifiListCallbacks());
  wifiListCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void BleManager::sendWifiList(std::vector<WifiModel> models) {
  partsToSend.clear();

  String json = jsonCoder.encodeWifiNameList(models);

  int partSize = 200;
  int partsCount = json.length() / partSize;

  for (int i = 0; i <= partsCount; i++) {
    String jsonPart = json.substring(i * partSize, (i + 1) * partSize);
    partsToSend.push_back(jsonPart);
  }

  nextPart = "START";
  wifiListCharacteristic->notify();
}