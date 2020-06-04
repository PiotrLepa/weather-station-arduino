#include "rest_client.h"

RestClient::RestClient(String _baseUrl, String wifiSsid, String wifiPassword) {
  baseUrl = _baseUrl;
  connectToWifi(wifiSsid, wifiPassword);
}

int RestClient::get(String path, String* response) {
  return sendRequest("GET", path, NULL, response);
}

int RestClient::post(String path, String body) {
  return sendRequest("POST", path, body.c_str(), NULL);
}

int RestClient::post(String path, String body, String* response) {
  return sendRequest("POST", path, body.c_str(), response);
}

int RestClient::put(String path, String body) {
  return sendRequest("PUT", path, body.c_str(), NULL);
}

int RestClient::put(String path, String body, String* response) {
  return sendRequest("PUT", path, body.c_str(), response);
}

int RestClient::sendRequest(String method, String path, const char* body,
                            String* response) {
  HTTPClient http;

  http.begin(baseUrl + path);
  http.addHeader("Content-Type", "application/json");

  int responseCode = http.sendRequest(method.c_str(), body);
  if (response != NULL) {
    response->concat(http.getString());
  }

  http.end();

  return responseCode;
}

void RestClient::connectToWifi(String ssid, String password) {
  REST_CLIENT_DEBUG_PRINT("Connect to WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    REST_CLIENT_DEBUG_PRINT(".");
  }

  REST_CLIENT_DEBUG_PRINT("Connected!");
}