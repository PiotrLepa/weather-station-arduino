#include "sd_card_storage.h"

bool SdCardStorage::begin() { return SD.begin(); }

bool SdCardStorage::createFileDirs(String path) {
  int dirsEnd = path.lastIndexOf("/");
  return SD.mkdir(path.substring(0, dirsEnd));
}

void SdCardStorage::write(String path, String value) {
  createFileDirs(path);

  File file = SD.open(path, FILE_WRITE);
  file.print(value);
  file.close();
}

String SdCardStorage::readFromFile(File file) {
  String value = "";
  while (file.available()) {
    value += (char)file.read();
  }
  return value;
}

std::vector<String> SdCardStorage::readAllInDirectory(String path) {
  std::vector<String> filesData;

  if (!SD.exists(path)) return filesData;

  File dir = SD.open(path, FILE_READ);
  dir.rewindDirectory();

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

  // TODO check if can be moved to upper loop
  for (String name : fileNames) {
    SD.remove(name);
  }
}