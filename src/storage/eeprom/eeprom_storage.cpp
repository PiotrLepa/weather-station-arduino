#include "eeprom_storage.h"

void EepromStorage::begin() { EEPROM.begin(EEPROM_SIZE); }

void EepromStorage::write(String value, int address) {
  EEPROM.writeString(address, value);
  EEPROM.commit();
}

String EepromStorage::read(int address) { return EEPROM.readString(address); }