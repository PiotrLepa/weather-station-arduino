#ifndef BLE_CALLBACKS_H
#define BLE_CALLBACKS_H

#include "../../model/wifi_credentials/wifi_credentials_model.h"
#include "../wifi_client/wifi_client.h"

class BleCallbacks {
 public:
  virtual void scanAvailablesWifi();
  virtual ConnectionResult connectToWifi(WifiCredentialsModel credentials);
};

#endif