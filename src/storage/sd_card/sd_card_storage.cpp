#include "sd_card_storage.h"

bool SdCardStorage::begin() {
  bool initialized = false;
  while (!initialized) {
    initialized = SD.begin();
    Serial.print("SD Card initialization result: ");
    Serial.println(initialized);
    delay(5000);
  }
  return true;
}

bool SdCardStorage::createFileDirs(String path) {
  int dirsEnd = path.lastIndexOf("/");
  return SD.mkdir(path.substring(0, dirsEnd));
}

void SdCardStorage::write(String path, String value) {
  createFileDirs(path);

  File file = SD.open(path, FILE_WRITE);
  // size_t writtenBytes = file.print(value);
  file.print(value);
  file.close();
  // Serial.print("SD Card written bytes: ");
  // Serial.println(writtenBytes);
  // if (writtenBytes == 0) {
  //   begin();
  //   write(path, value);
  // }
}

String SdCardStorage::readFromFile(File file) {
  String value = "";
  while (file.available()) {
    value += (char)file.read();
  }
  return value;
}

std::vector<String> SdCardStorage::readAllInDirectory(String path) {
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

void SdCardStorage::removeAllInDirectory(String path) {
  File dir = SD.open(path, FILE_READ);
  dir.rewindDirectory();

  std::vector<String> fileNames;
  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      // no more files
      break;
    }
    fileNames.push_back(file.name());
    file.close();
  }

  for (String name : fileNames) {
    SD.remove(name);
  }
}