#include "sd_card_manager.h"

SdCardManager::SdCardManager() {}

bool SdCardManager::begin() {
  return SD.begin();
}

bool SdCardManager::createFileDirs(String path) {
  int dirsEnd = path.lastIndexOf("/");
  Serial.println(path.substring(0, dirsEnd));
  return SD.mkdir(path.substring(0, dirsEnd));
}

void SdCardManager::write(String path, String value) {
  createFileDirs(path);
  
  File file = SD.open(path, FILE_WRITE);
  file.print(value);
  file.close();
}

String SdCardManager::readFromFile(File file) {
  String value = "";
  while (file.available()) {
    value += (char)file.read();
  }
  return value;
}

std::vector<String> SdCardManager::readAllInDirectory(String path) {
  File dir = SD.open(path, FILE_READ);
  dir.rewindDirectory();

  std::vector<String> filesData;
  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      // no more files
      break;
    }

    filesData.push_back(readFromFile(file));

    file.close();
  }

  return filesData;
}