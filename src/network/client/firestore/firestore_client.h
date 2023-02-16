#ifndef FIRESTORE_CLIENT_H
#define FIRESTORE_CLIENT_H

#include <Arduino.h>
#include <Firebase_ESP_Client.h>

#include "../../../model/weather/weather_model.h"
#include "../../../utils/date_time/date_time.h"
#include "../../../utils/logger/logger.h"

#define FIREBASE_TIMEOUT 30000

#define WEATHERS_PATH "weathers"
#define SAVED_DAYS "savedDays"

class FirestoreClient {
 public:
  FirestoreClient(String projectId);

  void connect();
  bool saveWeather(WeatherModel weather);
  bool saveDay(DateTime day);

 private:
  String projectId;
};

#endif