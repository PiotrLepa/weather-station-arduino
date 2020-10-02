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

  void onRead(BLECharacteristic *pCharacteristic) {
    callbacks->scanAvailablesWifi();
  }
};

BleManager::BleManager(BleCallbacks *_callbacks) { callbacks = _callbacks; }

void BleManager::begin() {
  BLEDevice::init(BLE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());
  pCharacteristic->setValue(
      "[{\"name\": \"test1\"},{\"name\": \"test2\"}, {\"name\": \"test3\"}]");

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}