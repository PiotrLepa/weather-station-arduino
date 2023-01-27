#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SPIFFS.h>

#include "../../storage/file_manager/file_manager.h"

#define LOGS_DIR_NAME "logs"
#define LOGS_FILE_NAME "log"
#define MAX_LOGS_FILES 10

class LoggerImpl {
 public:
  LoggerImpl(FileManager _fileManager);

  bool begin(bool formatSavedLogsOnFail);
  void log(String log);
  void printSavedLogs();

 private:
  FileManager fileManager;

  String createFileName(int fileNumber);
  String readSavedLogs();
};

extern LoggerImpl LOGGER;

#endif