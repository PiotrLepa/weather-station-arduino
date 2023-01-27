#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <WiFi.h>

#include "../../utils/logger/logger.h"

enum ConnectionResult { CONNECTED, ERROR };

class WifiClient {
 public:
  void begin();
  ConnectionResult connectToWifi(String ssid, String password, int tries);

 private:
  void restartWifi();
  bool isWifiConnected();
};

#endif