#include "file_manager.h"

FileManager::FileManager(FS* _fileSystem) : fileSystem(_fileSystem) {}

String FileManager::readFile(String path) {
  File file = fileSystem->open(path, FILE_READ);
  return readFile(file);
}

String FileManager::readFile(File file) {
  String value = "";
  while (file.available()) {
    value += (char)file.read();
  }
  return value;
}

std::vector<String> FileManager::readAllInDirectory(String path) {
  std::vector<String> filesData;

  if (!fileSystem->exists(path)) return filesData;

  File dir = fileSystem->open(path, FILE_READ);
  dir.rewindDirectory();

  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      // no more files
      break;
    }
    filesData.push_back(readFile(file));
    file.close();
  }

  return filesData;
}

bool FileManager::write(String path, String value) { write(path, value, FILE_WRITE); }

bool FileManager::append(String path, String value) { write(path, value, FILE_APPEND); }

bool FileManager::write(String path, String value, String mode) {
  File file = fileSystem->open(path, mode.c_str(), true);
  if (file) {
    file.print(value);
    file.close();
    return true;
  } else {
    return false;
  }
}

void FileManager::removeAllInDirectory(String path) {
  File dir = fileSystem->open(path, FILE_READ);
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
    fileSystem->remove(name);
  }
}