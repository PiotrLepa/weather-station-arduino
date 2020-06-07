#include "pm_controller.h"

PmController::PmController(HardwareSerial &_serial) : pms(PMSx003, _serial) {}

void PmController::begin() { pms.init(); }

bool PmController::read() {
  pms.read();
  return pms;
}

PmModel PmController::getData() {
  return PmModel(pms.pm01, pms.pm25, pms.pm10);
}

String PmController::getErrorMessage() {
  switch (pms.status) {
    case SerialPM::STATUS::OK:
      return "No errors";
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
  }
}