#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>
#include <FS.h>

#include <vector>

const String TXT_EXT = ".txt";

class FileManager {
 public:
  FileManager(FS* _fileSystem);

  String readFile(String path);
  String readFile(File file);
  std::vector<String> readAllInDirectory(String path);
  bool write(String path, String value);
  bool append(String path, String value);
  void removeAllInDirectory(String path);

 private:
  FS* fileSystem;

  bool write(String path, String value, String mode);
};

#endif