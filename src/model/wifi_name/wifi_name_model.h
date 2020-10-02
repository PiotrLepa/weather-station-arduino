#ifndef WIFI_NAME_MODEL_H
#define WIFI_NAME_MODEL_H

#include <Arduino.h>

struct WifiNameModel {
  String name;

  WifiNameModel(String _name) : name(_name) {}
};

#endif