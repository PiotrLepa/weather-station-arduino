#ifndef SD_CARD_STORAGE_H
#define SD_CARD_STORAGE_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <vector>

const String TXT_EXT = ".txt";
const String CACHED_WEATHERS_PATH = "/cached_weathers";

class SdCardStorage {
 public:
  bool begin();
  bool createFileDirs(String path);
  void write(String path, String value);
  String readFromFile(File file);
  std::vector<String> readAllInDirectory(String path);
  void removeAllInDirectory(String path);
};

#endif