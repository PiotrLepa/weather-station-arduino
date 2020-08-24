#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

class RestClient {
 public:
  RestClient(String apiUrl);

  int get(String path, String* response);

  int post(String path, String body);
  int post(String path, String body, String* response);

  int put(String path, String body);
  int put(String path, String body, String* response);

  IPAddress connectToWifi(const char* ssid, const char* password);

 private:
  String baseUrl;

  int sendRequest(String method, String path, const char* body,
                  String* response);
};

#endif