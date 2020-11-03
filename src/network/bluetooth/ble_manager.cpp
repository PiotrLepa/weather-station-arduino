#include "ble_manager.h"

BleManager *instance;

class ServerCallbacks : public BLEServerCallbacks {
  BleManager *bleManager;

 public:
  ServerCallbacks(BleManager *_bleManager) : bleManager(_bleManager) {}

  void onConnect(BLEServer *server) { instance->startDisconnectTimer(); }

  void onDisconnect(BLEServer *server) { instance->stopDisconnectTimer(); }
};

class ScanWifiCallbacks : public BLECharacteristicCallbacks {
  BleManager *bleManager;

 public:
  ScanWifiCallbacks(BleManager *_bleManager) : bleManager(_bleManager) {}

  void onRead(BLECharacteristic *pCharacteristic) {
    instance->restartDisconnectTimer();

    bleManager->callbacks->scanAvailablesWifi();
  }
};

class WifiListCallbacks : public BLECharacteristicCallbacks {
  BleManager *bleManager;

 public:
  WifiListCallbacks(BleManager *_bleManager) : bleManager(_bleManager) {}

  void onWrite(BLECharacteristic *characteristic) {
    instance->restartDisconnectTimer();

    std::string json = characteristic->getValue();
    WifiCredentialsModel credentials =
        bleManager->jsonCoder.decodeWifiCredentials(json.c_str());
    ConnectionResult result = bleManager->callbacks->connectToWifi(credentials);
    bleManager->sendConnectToWifiResult(result);
  }

  void onRead(BLECharacteristic *characteristic) {
    instance->restartDisconnectTimer();

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

void onDisconnect() { instance->disconnect(); }

BleManager::BleManager(JsonCoder _jsonCoder)
    : jsonCoder(_jsonCoder), disconnectTimer(onDisconnect, DISCONNECT_DELAY) {
  instance = this;
}

void BleManager::begin(BleCallbacks *_callbacks) {
  callbacks = _callbacks;

  BLEDevice::init(BLE_NAME);
  server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks(this));

  BLEService *service = server->createService(SERVICE_UUID);
  setupScanWifiCharacteristic(service);
  setupWifiListCharacteristic(service);
  setupConnectToWifiResultCharacteristic(service);
  service->start();

  server->startAdvertising();
}

void BleManager::update() { disconnectTimer.update(); }

void BleManager::startDisconnectTimer() { disconnectTimer.start(); }

void BleManager::stopDisconnectTimer() { disconnectTimer.stop(); }

void BleManager::restartDisconnectTimer() {
  disconnectTimer.stop();
  disconnectTimer.start();
}

void BleManager::disconnect() {
  disconnectTimer.stop();
  server->disconnect(server->getConnId());
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
      Serial.println("Wifi status: connected");
      connectToWifiResultCharacteristic->setValue("connected");
      break;
    case ERROR:
      Serial.println("Wifi status: error");
      connectToWifiResultCharacteristic->setValue("error");
      break;
  }
  connectToWifiResultCharacteristic->notify();
}