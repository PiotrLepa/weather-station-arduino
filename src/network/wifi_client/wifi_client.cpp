#include "wifi_client.h"

void onWifiConnectedAndIpAddressGot(WiFiEvent_t event, WiFiEventInfo_t info) {
  LOGGER.log("Wifi connected, ip address: " + WiFi.localIP().toString());
  LOGGER.log("RSSI: " + String(WiFi.RSSI()));
}

void onWifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) { LOGGER.log("Wifi disconnected"); }

void WifiClient::begin() {
  WiFi.onEvent(onWifiConnectedAndIpAddressGot, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(onWifiDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  restartWifi();
}

ConnectionResult WifiClient::connectToWifi(String ssid, String password, int tries) {
  Serial.println("Connecting to WiFi");

  if (password == "null") {
    WiFi.begin(ssid.c_str());
  } else {
    WiFi.begin(ssid.c_str(), password.c_str());
  }

  ConnectionResult result = ERROR;
  for (int i = 0; i < tries; i++) {
    if (WifiClient::isWifiConnected()) {
      result = CONNECTED;
      break;
    } else {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.print("");

  WiFi.setAutoReconnect(true);

  Serial.println("Wifi status: " + String(WiFi.status()));

  return result;
}

bool WifiClient::isWifiConnected() { return WiFi.status() == WL_CONNECTED; }

void WifiClient::restartWifi() {
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
}