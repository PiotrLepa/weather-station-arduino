#ifndef REST_CLIENT_H

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#ifdef RESUT_CLIENT
#define RESUT_CLIENT_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifndef RESUT_CLIENT_DEBUG
#define RESUT_CLIENT_DEBUG_PRINT(string)
#endif

class RestClient {
 public:
  RestClient(String apiUrl, String wifiSsid, String wifiPassword);

  int get(String path, String* response);

  int post(String path, String body);
  int post(String path, String body, String* response);

  int put(String path, String body);
  int put(String path, String body, String* response);

 private:
  String baseUrl;

  int sendRequest(String method, String path, const char* body,
                  String* response);
  void connectToWifi(String ssid, String password);
};

#endif