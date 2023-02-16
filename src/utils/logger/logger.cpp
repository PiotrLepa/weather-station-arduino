#include "logger.h"

LoggerImpl LOGGER = LoggerImpl(&SPIFFS);

LoggerImpl::LoggerImpl(FileManager _fileManager) : fileManager(_fileManager) {}

bool LoggerImpl::begin() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }

  File root = SPIFFS.open(LOGS_ROOT_DIR);
  File file = root.openNextFile();
  while (file) {
    Serial.print("Logs file: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  return true;
}

void LoggerImpl::log(String log) {
  Serial.println("LOGGER: " + log);

  // TODO Consider refactor to one file
  int tries = 0;
  while (tries <= MAX_LOGS_FILES) {
    bool writeSuccessfull = fileManager.append(createFileName(tries), formatLogToSave(log));
    tries++;
    if (writeSuccessfull) {
      break;
    }

    Serial.println("Saving logs failed. Retrying " + tries);
  }
}

String LoggerImpl::formatLogToSave(String log) {
  String timestamp = DateTime::now().getFormattedDateTime();
  String logPrefix = "";
  if (timestamp != "") {
    logPrefix = timestamp + " ";
  }
  return logPrefix + log + "\n";
}

void LoggerImpl::printSavedLogs() {
  String savedLogs = readSavedLogs();
  if (savedLogs != "") {
    Serial.println("\n--------- Saved logs START ---------\n");
    Serial.println(savedLogs);
    Serial.println("--------- Saved logs END ---------\n");
  } else {
    Serial.println("\n--------- No saved logs ---------\n");
  }
}

String LoggerImpl::readSavedLogs() {
  String joinedLogs = "";

  std::vector<String> logs = fileManager.readAllInDirectory(LOGS_ROOT_DIR);
  for (String log : logs) {
    joinedLogs += log + "\n";
  }

  return joinedLogs;
}

bool LoggerImpl::clear() {
  Serial.println("\n--------- Saved logs cleared ---------\n");
  return SPIFFS.format();
}

String LoggerImpl::createFileName(int fileNumber) { return String(LOGS_ROOT_DIR) + "/" + LOGS_FILE_NAME + fileNumber + TXT_EXT; }