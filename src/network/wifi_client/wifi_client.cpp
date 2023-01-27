#include "wifi_client.h"

void WifiClient::begin() { restartWifi(); }

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
      Serial.print("\nConnected! Ip address: ");
      Serial.println(WiFi.localIP());

      result = CONNECTED;
      break;
    } else {
      delay(500);
      Serial.print(".");
    }
  }

  LOGGER.log("Wifi status: " + String(WiFi.status()));

  return result;
}

bool WifiClient::isWifiConnected() { return WiFi.status() == WL_CONNECTED; }

void WifiClient::restartWifi() {
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
}