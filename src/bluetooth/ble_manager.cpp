#include "ble_manager.h"

BleCallbacks *callbacks;

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    callbacks->connectToWifi();
    // if (value.length() > 0) {
    //   Serial.println("*********");

    //   Serial.println("old: ");
    //   Serial.println(value.c_str());
    //   Serial.print("New value: ");
    //   for (int i = 0; i < value.length(); i++) Serial.print(value[i]);

    //   Serial.println();
    //   Serial.println("*********");
    // }
  }

  // void onRead(BLECharacteristic *pCharacteristic) {
  //   callbacks->scanAvailablesWifi();
  // }
};

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { callbacks->scanAvailablesWifi(); };

  void onDisconnect(BLEServer *pServer) {}
};

BleManager::BleManager(JsonCoder _jsonCoder) : jsonCoder(_jsonCoder) {}

void BleManager::begin(BleCallbacks *_callbacks) {
  callbacks = _callbacks;

  BLEDevice::init(BLE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY |
                               BLECharacteristic::PROPERTY_READ |
                               BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void BleManager::sendAvailableWifiList(std::vector<WifiNameModel> models) {
  String json = jsonCoder.encodeWifiNameList(models);
  pCharacteristic->setValue(json.c_str());
  Serial.println("NOTIFY");
  pCharacteristic->notify();
}