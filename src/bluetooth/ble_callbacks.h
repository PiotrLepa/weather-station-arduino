#ifndef BLE_CALLBACKS_H
#define BLE_CALLBACKS_H

class BleCallbacks {
 public:
  virtual void scanAvailablesWifi();
  virtual void connectToWifi();
};

#endif