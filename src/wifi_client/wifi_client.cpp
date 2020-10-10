#include "wifi_client.h"

IPAddress WifiClient::connectToWifi(const char* ssid, const char* password) {
  Serial.println("Connecting to WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nConnected! Ip address: ");
  Serial.println(WiFi.localIP());

  return WiFi.localIP();
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
  return WifiModel(WiFi.SSID(index), mapEncryption(WiFi.encryptionType(index)),
                   WiFi.RSSI(index));
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