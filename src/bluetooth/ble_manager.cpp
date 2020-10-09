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
  void onWrite(BLECharacteristic *characteristic) {
    std::string json = characteristic->getValue();
    WifiCredentialsModel credentials = instance->jsonCoder.decodeWifiCredentials(json.c_str());
    instance->callbacks->connectToWifi(credentials);
  }

  void onRead(BLECharacteristic *characteristic) {
    characteristic->setValue(instance->nextPart.c_str());

    if (instance->nextPart == "START") {
      instance->nextPart = instance->partsToSend.front();
      instance->partsToSend.pop_front();
      characteristic->notify();
    } else if (instance->nextPart == "END") {
      instance->nextPart = "START";
    } else {
      if (instance->partsToSend.empty()) {
        instance->nextPart = "END";
      } else {
        instance->nextPart = instance->partsToSend.front();
        instance->partsToSend.pop_front();
      }

      characteristic->notify();
    }
  }
};

BleManager::BleManager(JsonCoder _jsonCoder) : jsonCoder(_jsonCoder) {
  instance = this;
}

void BleManager::begin(BleCallbacks *_callbacks) {
  callbacks = _callbacks;

  BLEDevice::init(BLE_NAME);
  BLEServer *server = BLEDevice::createServer();
  // pServer->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);
  setupScanWifiCharacetistic(service);
  setupWifiListCharacetistic(service);
  service->start();

  BLEAdvertising *advertising = server->getAdvertising();
  advertising->start();
}

void BleManager::sendWifiList(std::vector<WifiModel> models) {
  partsToSend.clear();

  String json = jsonCoder.encodeWifiNameList(models);

  int partsCount = json.length() / PART_SIZE;

  for (int i = 0; i <= partsCount; i++) {
    int startIndex = i * PART_SIZE;
    String jsonPart = json.substring(startIndex, startIndex + PART_SIZE);
    partsToSend.push_back(jsonPart);
  }

  nextPart = "START";
  wifiListCharacteristic->notify();
}

void BleManager::setupScanWifiCharacetistic(BLEService *service) {
  BLECharacteristic *startScanCharacteristic = service->createCharacteristic(
      SCAN_WIFI_CHARACTERISTIC, BLECharacteristic::PROPERTY_READ);
  startScanCharacteristic->setCallbacks(new ScanWifiCallbacks());
  startScanCharacteristic->addDescriptor(new BLE2902());
  startScanCharacteristic->setValue("OK");
}

void BleManager::setupWifiListCharacetistic(BLEService *service) {
  wifiListCharacteristic = service->createCharacteristic(
      WIFI_LIST_CHARACTERISTIC, BLECharacteristic::PROPERTY_NOTIFY |
                                    BLECharacteristic::PROPERTY_READ |
                                    BLECharacteristic::PROPERTY_WRITE);
  wifiListCharacteristic->setCallbacks(new WifiListCallbacks());
  wifiListCharacteristic->addDescriptor(new BLE2902());
}