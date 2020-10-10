#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <WiFi.h>

#include <list>

#include "../model/wifi/wifi_model.h"

class WifiClient {
 public:
  IPAddress connectToWifi(String ssid, String password);
  std::vector<WifiModel> scanWifi();

 private:
  WifiModel getWifiInfo(int index);
  WifiEncryption mapEncryption(wifi_auth_mode_t auth);
};

#endif