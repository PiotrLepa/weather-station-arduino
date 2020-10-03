#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <WiFi.h>

#include "../model/wifi/wifi_model.h"

class WifiClient {
 public:
  IPAddress connectToWifi(const char* ssid, const char* password);
  std::vector<WifiModel> scanWifi();

 private:
  WifiModel getWifiInfo(int index);
  WifiEncryption mapEncryption(wifi_auth_mode_t auth);
};

#endif