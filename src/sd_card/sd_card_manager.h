#ifndef SD_CARD_MANAGER_H
#define SD_CARD_MANAGER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SdCardManager {
 public:
  SdCardManager();

  bool begin();
  bool createFileDirs(String path);
  void write(String path, String value);
  String readFromFile(File file);
  std::vector<String> readAllInDirectory(String path);

 private:
};

#endif