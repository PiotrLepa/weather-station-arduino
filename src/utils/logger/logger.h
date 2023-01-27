#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SPIFFS.h>

#include "../../storage/file_manager/file_manager.h"
#include "../date_time/date_time.h"

#define LOGS_ROOT_DIR "/"  // SPIFFS does not support directories
#define LOGS_FILE_NAME "log"
#define MAX_LOGS_FILES 10

class LoggerImpl {
 public:
  LoggerImpl(FileManager _fileManager);

  bool begin();
  void log(String log);
  void printSavedLogs();
  bool clear();

 private:
  FileManager fileManager;

  String formatLogToSave(String log);
  String createFileName(int fileNumber);
  String readSavedLogs();
};

extern LoggerImpl LOGGER;

#endif