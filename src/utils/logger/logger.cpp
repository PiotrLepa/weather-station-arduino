#include "logger.h"

LoggerImpl LOGGER = LoggerImpl(&SPIFFS);

LoggerImpl::LoggerImpl(FileManager _fileManager) : fileManager(_fileManager) {}

bool LoggerImpl::begin(bool formatSavedLogsOnFail) {
  if (!SPIFFS.begin(formatSavedLogsOnFail)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }
  return true;
}

void LoggerImpl::log(String log) {
  Serial.println(log);

  int tries = 0;
  while (tries <= MAX_LOGS_FILES) {
    bool writeSuccessfull = fileManager.append(createFileName(tries), log);
    tries++;
    if (writeSuccessfull) {
      break;
    }
  }
}

void LoggerImpl::printSavedLogs() { Serial.println(readSavedLogs()); }

String LoggerImpl::readSavedLogs() {
  String joinedLogs = "";

  std::vector<String> logs = fileManager.readAllInDirectory(LOGS_DIR_NAME);
  for (String log : logs) {
    joinedLogs += log + "\n";
  }

  return joinedLogs;
}

String LoggerImpl::createFileName(int fileNumber) { return String(LOGS_DIR_NAME) + "/" + LOGS_FILE_NAME + fileNumber + TXT_EXT; }