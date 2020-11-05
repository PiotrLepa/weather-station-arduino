#include "wifi_client.h"

ConnectionResult WifiClient::connectToWifi(String ssid, String password) {
  Serial.println("Connecting to WiFi");

  WiFi.begin(ssid.c_str(), password.c_str());

  ConnectionResult result = ERROR;
  for (int i = 0; i < 5; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("\nConnected! Ip address: ");
      Serial.println(WiFi.localIP());

      result = CONNECTED;
      break;
    } else {
      delay(500);
      Serial.print(".");
    }
  }

  return result;
}

std::vector<WifiModel> WifiClient::scanWifi() {
  int n = WiFi.scanNetworks();

  std::vector<WifiModel> wifiList;
  for (int i = 0; i < n; i++) {
    WifiModel wifi = getWifiInfo(i);
    wifiList.push_back(wifi);
  }
  return wifiList;
}

WifiModel WifiClient::getWifiInfo(int index) {
  return WifiModel(WiFi.SSID(index), mapEncryption(WiFi.encryptionType(index)), WiFi.RSSI(index));
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