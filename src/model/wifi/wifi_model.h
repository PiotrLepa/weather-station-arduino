#ifndef WIFI_MODEL_H
#define WIFI_MODEL_H

#include <Arduino.h>

enum WifiEncryption { OPEN, WEP, WPA, WPA2 };

inline const char* encryptionToString(WifiEncryption encryption) {
  switch (encryption) {
    case OPEN:
      return "open";
    case WEP:
      return "wep";
    case WPA:
      return "wpa";
    case WPA2:
      return "wpa2";
    default:
      return "open";
  }
}

struct WifiModel {
  String name;
  WifiEncryption encryption;
  int rssi;

  WifiModel(String _name, WifiEncryption _encryption, int _rssi)
      : name(_name), encryption(_encryption), rssi(_rssi) {}
};

#endif