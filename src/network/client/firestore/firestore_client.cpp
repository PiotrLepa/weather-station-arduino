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
    content.set("fields/pressure/doubleValue", weather.pressure.pressure);
    content.set("fields/pm1/integerValue", weather.airQuality.pm1);
    content.set("fields/pm25/integerValue", weather.airQuality.pm25);
    content.set("fields/pm10/integerValue", weather.airQuality.pm10);
    content.set("fields/windSpeedMax/doubleValue", weather.wind.windSpeedMax);
    content.set("fields/windSpeedAvg/doubleValue", weather.wind.windSpeedAvg);
    content.set("fields/rainGauge/doubleValue", weather.rainGauge.amountOfPrecipitation);
    content.set("fields/myTimestamp/timestampValue", weather.timestamp);

    String documentPath = "weathers";

    firebaseData.setResponseSize(1024);
    if (Firebase.Firestore.createDocument(&firebaseData, projectId, "", documentPath.c_str(), content.raw()))
        Serial.printf("Document created");
    else
        Serial.println(firebaseData.errorReason());
    return true;
}