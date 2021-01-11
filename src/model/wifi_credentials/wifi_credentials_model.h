#ifndef WIFI_CREDENTIALS_MODEL_H
#define WIFI_CREDENTIALS_MODEL_H

#include <Arduino.h>

struct WifiCredentialsModel {
  String name;
  String password;

  WifiCredentialsModel(String _name, String _password) : name(_name), password(_password) {}
};

#endif