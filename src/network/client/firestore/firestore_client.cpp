#include "firestore_client.h"

FirestoreClient::FirestoreClient(String _projectId) : projectId(_projectId)
{
}

unsigned long dataMillis = 0;

bool FirestoreClient::write(WeatherModel weather)
{
  FirebaseConfig config;

  String databaseUrl = projectId;
  databaseUrl += ".firebaseio.com";
  config.database_url = databaseUrl;

  config.signer.test_mode = true;

  FirebaseAuth auth;

  Firebase.begin(&config, &auth);

  FirebaseJson content;

  content.set("fields/temperature/doubleValue", weather.externalTemperature.temperature);
  content.set("fields/humidity/doubleValue", weather.pressure.humidity);
  content.set("fields/pressure/doubleValue", weather.pressure.pressure);
  content.set("fields/pm1/integerValue", weather.airQuality.pm1);
  content.set("fields/pm25/integerValue", weather.airQuality.pm25);
  content.set("fields/pm10/integerValue", weather.airQuality.pm10);
  content.set("fields/windSpeedMax/doubleValue", weather.wind.windSpeedMax);
  content.set("fields/windSpeedAvg/doubleValue", weather.wind.windSpeedAvg);
  content.set("fields/rainGauge/doubleValue", weather.rainGauge.amountOfPrecipitation);
  // content.set("fields/myTimestamp/timestampValue", "2014-10-02T15:01:23Z"); // RFC3339 UTC "Zulu" format

  String documentPath = "weathers";

  FirebaseData firebaseData;
  if (Firebase.Firestore.createDocument(&firebaseData, projectId, "", documentPath.c_str(), content.raw()))
    Serial.printf("Document created");
  else
    Serial.println(firebaseData.errorReason());
  return true;
}