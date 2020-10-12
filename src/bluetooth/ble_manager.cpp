#include "ble_manager.h"

// class ServerCallbacks : public BLEServerCallbacks {
//   BleManager *bleManager;

//  public:
//   ServerCallbacks(BleManager *_bleManager) : bleManager(_bleManager) {}

//   void onConnect(BLEServer *server) {}

//   void onDisconnect(BLEServer *server) {}
// };

class ScanWifiCallbacks : public BLECharacteristicCallbacks {
  BleManager *bleManager;

 public:
  ScanWifiCallbacks(BleManager *_bleManager) : bleManager(_bleManager) {}

  void onRead(BLECharacteristic *pCharacteristic) {
    bleManager->callbacks->scanAvailablesWifi();
  }
};

class WifiListCallbacks : public BLECharacteristicCallbacks {
  BleManager *bleManager;

 public:
  WifiListCallbacks(BleManager *_bleManager) : bleManager(_bleManager) {}

  void onWrite(BLECharacteristic *characteristic) {
    std::string json = characteristic->getValue();
    WifiCredentialsModel credentials =
        bleManager->jsonCoder.decodeWifiCredentials(json.c_str());
    bleManager->callbacks->connectToWifi(credentials);
  }

  void onRead(BLECharacteristic *characteristic) {
    switch (bleManager->nextMessageType) {
      case START:
        bleManager->nextMessageType = PART;
        characteristic->setValue("start");
        characteristic->notify();
        break;
      case END:
        characteristic->setValue("end");
        break;
      case PART:
        characteristic->setValue(bleManager->partsToSend.front().c_str());
        bleManager->partsToSend.pop_front();
        if (bleManager->partsToSend.empty()) {
          bleManager->nextMessageType = END;
        }
        characteristic->notify();
        break;
    }
  }
};

BleManager::BleManager(JsonCoder _jsonCoder) : jsonCoder(_jsonCoder) {}

void BleManager::begin(BleCallbacks *_callbacks) {
  callbacks = _callbacks;

  BLEDevice::init(BLE_NAME);
  BLEServer *server = BLEDevice::createServer();
  // server->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);
  setupScanWifiCharacteristic(service);
  setupWifiListCharacteristic(service);
  service->start();

  BLEAdvertising *advertising = server->getAdvertising();
  advertising->start();
}

void BleManager::setupScanWifiCharacteristic(BLEService *service) {
  BLECharacteristic *characteristic = service->createCharacteristic(
      SCAN_WIFI_CHARACTERISTIC, BLECharacteristic::PROPERTY_READ);
  characteristic->setCallbacks(new ScanWifiCallbacks(this));
  characteristic->addDescriptor(new BLE2902());
}

void BleManager::setupWifiListCharacteristic(BLEService *service) {
  wifiListCharacteristic = service->createCharacteristic(
      WIFI_LIST_CHARACTERISTIC, BLECharacteristic::PROPERTY_NOTIFY |
                                    BLECharacteristic::PROPERTY_READ |
                                    BLECharacteristic::PROPERTY_WRITE);
  wifiListCharacteristic->setCallbacks(new WifiListCallbacks(this));
  wifiListCharacteristic->addDescriptor(new BLE2902());
}

void BleManager::setupConnectToWifiResultCharacteristic(BLEService *service) {
  connectToWifiResultCharacteristic =
      service->createCharacteristic(CONNECT_TO_WIFI_RESULT_CHARACTERISTIC,
                                    BLECharacteristic::PROPERTY_NOTIFY);
  connectToWifiResultCharacteristic->addDescriptor(new BLE2902());
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

  nextMessageType = START;
  wifiListCharacteristic->notify();
}

void BleManager::sendConnectToWifiResult(ConnectionResult status) {
  switch (status) {
    case CONNECTED:
      connectToWifiResultCharacteristic->setValue("connected");
      break;
    case ERROR:
      connectToWifiResultCharacteristic->setValue("error");
      break;
  }
  connectToWifiResultCharacteristic->notify();
}