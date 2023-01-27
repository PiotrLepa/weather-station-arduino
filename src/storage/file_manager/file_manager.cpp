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

  File root = fileSystem->open(path, FILE_READ);
  File file = root.openNextFile();
  while (file) {
    filesData.push_back(readFile(file));
    file.close();
    file = root.openNextFile();
  }

  return filesData;
}

bool FileManager::write(String path, String value) { return write(path, value, FILE_WRITE); }

bool FileManager::append(String path, String value) { return write(path, value, FILE_APPEND); }

bool FileManager::write(String path, String value, String mode) {
  if (mode == FILE_APPEND && !fileSystem->exists(path)) {
    mode = FILE_WRITE;
  }

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
  std::vector<String> fileNames;

  File root = fileSystem->open(path, FILE_READ);
  File file = root.openNextFile();
  while (file) {
    fileNames.push_back(file.name());
    file.close();
    file = root.openNextFile();
  }

  for (String name : fileNames) {
    fileSystem->remove(name);
  }
}