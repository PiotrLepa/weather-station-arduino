#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <WiFi.h>

#include <list>

#include "../../model/wifi/wifi_model.h"

enum ConnectionResult { CONNECTED, ERROR };

class WifiClient {
 public:
  void begin();
  ConnectionResult connectToWifi(String ssid, String password, int tries = 5);
  std::vector<WifiModel> scanWifi();
  bool isWifiConnected();

 private:
  WifiModel getWifiInfo(int index);
  WifiEncryption mapEncryption(wifi_auth_mode_t auth);
  void restartWifi();
};

#endif