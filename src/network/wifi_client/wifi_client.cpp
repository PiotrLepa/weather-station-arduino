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

  Serial.print("Wifi status: ");
  Serial.println(WiFi.status());

  return result;
}

std::vector<WifiModel> WifiClient::scanWifi() {
  restartWifi();

  int numberOfWifi = WiFi.scanNetworks();

  std::vector<WifiModel> wifiList;
  for (int i = 0; i < numberOfWifi; i++) {
    WifiModel wifi = getWifiInfo(i);
    wifiList.push_back(wifi);
  }
  return wifiList;
}

WifiModel WifiClient::getWifiInfo(int index) {
  return WifiModel(WiFi.SSID(index), mapEncryption(WiFi.encryptionType(index)), WiFi.RSSI(index));
}

bool WifiClient::isWifiConnected() { return WiFi.status() == WL_CONNECTED; }

void WifiClient::restartWifi() {
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
}

WifiEncryption WifiClient::mapEncryption(wifi_auth_mode_t auth) {
  switch (auth) {
    case WIFI_AUTH_OPEN:
      return OPEN;
    case WIFI_AUTH_WEP:
      return WEP;
    case WIFI_AUTH_WPA_PSK:
      return WPA;
    case WIFI_AUTH_WPA2_PSK:
    case WIFI_AUTH_WPA_WPA2_PSK:
    case WIFI_AUTH_WPA2_ENTERPRISE:
    case WIFI_AUTH_MAX:
      return WPA2;
    default:
      return OPEN;
  }
}