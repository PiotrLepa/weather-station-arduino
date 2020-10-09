#ifndef BLE_CALLBACKS_H
#define BLE_CALLBACKS_H

#include "../model/wifi_credentials/wifi_credentials_model.h"

class BleCallbacks {
 public:
  virtual void scanAvailablesWifi();
  virtual void connectToWifi(WifiCredentialsModel credentials);
};

#endif