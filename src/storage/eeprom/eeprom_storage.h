#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 512
#define WIFI_CREDENTIALS_ADDRESS 0

class EepromStorage {
 public:
  void begin();
  void write(String value, int address);
  String read(int address);
};

#endif