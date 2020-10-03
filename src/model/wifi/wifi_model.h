#ifndef WIFI_MODEL_H
#define WIFI_MODEL_H

#include <Arduino.h>

enum WifiEncryption { OPEN, WEP, WPA, WPA2 };

struct WifiModel {
  String name;
  WifiEncryption encryption;
  int rssi;

  WifiModel(String _name, WifiEncryption _encryption, int _rssi)
      : name(_name), encryption(_encryption), rssi(_rssi) {}
};

#endif