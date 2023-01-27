#include "air_quality_reader.h"

AirQualityReader::AirQualityReader(HardwareSerial &serial, uint8_t _pmsModePin) : pms(PMSx003, serial) {
  pmsModePin = _pmsModePin;
  pinMode(pmsModePin, OUTPUT);
}

bool AirQualityReader::begin() {
  pms.init();
  sleep();
  return true;
}

bool AirQualityReader::read() {
  pms.read();
  if (pms.pm10 == 0) {
    // Try read the data again
    pms.read();
    if (pms.pm10 == 0) {
      return false;
    }
  }

  return pms;
}

AirQualityModel AirQualityReader::getData() {
  if (pms.pm10 == -1 || pms.pm10 == 0) {
    return AirQualityModel::error();
  }

  return AirQualityModel(pms.pm01, pms.pm25, pms.pm10);
}

String AirQualityReader::getErrorMessage() {
  switch (pms.status) {
    case SerialPM::STATUS::OK:
      return "";
    case SerialPM::STATUS::ERROR_TIMEOUT:
      return PMS_ERROR_TIMEOUT;
    case SerialPM::STATUS::ERROR_MSG_UNKNOWN:
      return PMS_ERROR_MSG_UNKNOWN;
    case SerialPM::STATUS::ERROR_MSG_HEADER:
      return PMS_ERROR_MSG_HEADER;
    case SerialPM::STATUS::ERROR_MSG_BODY:
      return PMS_ERROR_MSG_BODY;
    case SerialPM::STATUS::ERROR_MSG_START:
      return PMS_ERROR_MSG_START;
    case SerialPM::STATUS::ERROR_MSG_LENGTH:
      return PMS_ERROR_MSG_LENGTH;
    case SerialPM::STATUS::ERROR_MSG_CKSUM:
      return PMS_ERROR_MSG_CKSUM;
    case SerialPM::STATUS::ERROR_PMS_TYPE:
      return PMS_ERROR_PMS_TYPE;
    default:
      return "";
  }
}

void AirQualityReader::wakeUp() { digitalWrite(pmsModePin, HIGH); }

void AirQualityReader::sleep() { digitalWrite(pmsModePin, LOW); }