#include "rest_client.h"

RestClient::RestClient(String _baseUrl) { baseUrl = _baseUrl; }

int RestClient::get(String path, String* response) { return sendRequest("GET", path, NULL, response); }

int RestClient::post(String path) { return sendRequest("POST", path, NULL, NULL); }

int RestClient::post(String path, String body) { return sendRequest("POST", path, body.c_str(), NULL); }

int RestClient::post(String path, String body, String* response) {
  return sendRequest("POST", path, body.c_str(), response);
}

int RestClient::put(String path, String body) { return sendRequest("PUT", path, body.c_str(), NULL); }

int RestClient::put(String path, String body, String* response) {
  return sendRequest("PUT", path, body.c_str(), response);
}

int RestClient::sendRequest(String method, String path, const char* body, String* response) {
  Serial.println("\n-------------------------------------------------------\n");
  Serial.print("url: ");
  Serial.println(baseUrl + path);
  Serial.print("method: ");
  Serial.println(method);
  Serial.print("body: ");
  Serial.println(body);

  HTTPClient httpClient;

  httpClient.begin(baseUrl + path);
  httpClient.addHeader("Content-Type", "application/json");
  int responseCode = httpClient.sendRequest(method.c_str(), body);
  if (response != NULL) {
    response->concat(httpClient.getString());
  }

  httpClient.end();

  Serial.print("responseCode: ");
  Serial.println(responseCode);
  Serial.println("\n-------------------------------------------------------\n");

  return responseCode;
}