#include "firestore_client.h"

FirebaseConfig config;
FirebaseAuth auth;
FirebaseData firebaseData;

FirestoreClient::FirestoreClient(String _projectId) : projectId(_projectId) {
  String databaseUrl = projectId;
  databaseUrl += ".firebaseio.com";
  config.database_url = databaseUrl;
  config.timeout.socketConnection = FIREBASE_TIMEOUT;
  config.timeout.serverResponse = FIREBASE_TIMEOUT;

  config.signer.test_mode = true;
}

void FirestoreClient::connect() { Firebase.begin(&config, &auth); }

bool FirestoreClient::saveWeather(WeatherModel weather) {
  connect();

  FirebaseJson content;
  content.set("fields/temperature/doubleValue", weather.externalTemperature.temperature);
  content.set("fields/humidity/doubleValue", weather.pressure.humidity);
  content.set("fields/pressure/integerValue", weather.pressure.pressure);
  content.set("fields/pm1/integerValue", weather.airQuality.pm1);
  content.set("fields/pm25/integerValue", weather.airQuality.pm25);
  content.set("fields/pm10/integerValue", weather.airQuality.pm10);
  content.set("fields/windSpeedMax/doubleValue", weather.wind.windSpeedMax);
  content.set("fields/windSpeedAvg/doubleValue", weather.wind.windSpeedAvg);
  content.set("fields/precipitation/doubleValue", weather.rainGauge.precipitation);
  content.set("fields/location/stringValue", "OUTSIDE");
  content.set("fields/timestamp/timestampValue", weather.timestamp.getFormattedDateTime());

  bool isSuccessful = Firebase.Firestore.createDocument(&firebaseData, projectId, "", WEATHERS_PATH, content.raw());
  if (!isSuccessful) {
    LOGGER.log("Firestore save weather error: " + String(firebaseData.errorReason().c_str()));
  }

  return isSuccessful;
}

bool FirestoreClient::saveDay(DateTime timestamp) {
  connect();

  FirebaseJson content;
  content.set("fields/day/timestampValue", timestamp.getFormattedDateTime());

  String documentPath =
      String(SAVED_DAYS) + "/" + String(timestamp.getYear()) + "/" + String(timestamp.getMonth()) + "/" + String(timestamp.getDay());

  bool isSuccessful = Firebase.Firestore.createDocument(&firebaseData, projectId, "", documentPath.c_str(), content.raw());
  if (!isSuccessful) {
    String errorMessage = String(firebaseData.errorReason().c_str());
    if (errorMessage == "Document already exists: projects/" + projectId + "/databases/(default)/documents/" + documentPath) {
      return true;
    } else {
      LOGGER.log("Firestore save day error: " + errorMessage);
    }
  }

  return isSuccessful;
}