#ifndef FIRESTORE_CLIENT_H
#define FIRESTORE_CLIENT_H

#include <Arduino.h>
#include <Firebase_ESP_Client.h>

#include "../../../model/weather/weather_model.h"
#include "../../../utils/logger/logger.h"

class FirestoreClient {
 public:
  FirestoreClient(String projectId);

  void connect();
  bool write(WeatherModel weather);

 private:
  String projectId;
};

#endif