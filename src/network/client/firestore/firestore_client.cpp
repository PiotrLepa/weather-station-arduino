#include "firestore_client.h"

FirebaseConfig config;
FirebaseAuth auth;
FirebaseData firebaseData;

FirestoreClient::FirestoreClient(String _projectId) : projectId(_projectId) {
  String databaseUrl = projectId;
  databaseUrl += ".firebaseio.com";
  config.database_url = databaseUrl;

  config.signer.test_mode = true;
}

void FirestoreClient::connect() { Firebase.begin(&config, &auth); }

bool FirestoreClient::write(WeatherModel weather) {
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
  content.set("fields/timestamp/timestampValue", weather.timestamp);

  String documentPath = "weathers";

  bool result = Firebase.Firestore.createDocument(&firebaseData, projectId, "", documentPath.c_str(), content.raw());
  if (!result) {
    LOGGER.log(firebaseData.errorReason());
  }

  return result;
}