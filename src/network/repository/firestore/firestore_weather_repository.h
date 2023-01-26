#ifndef FIRESTORE_WEATHER_REPOSITORY_H
#define FIRESTORE_WEATHER_REPOSITORY_H

#include <HTTPClient.h>

#include "../../../model/weather/weather_model.h"
#include "../../../storage/sd_card/sd_card_storage.h"
#include "../../../utils/date_time/date_time.h"
#include "../../../utils/json_coder/json_coder.h"
#include "../../client/firestore/firestore_client.h"
#include "../weather_repository.h"

class FirestoreWeatherRepository : public WeatherRepository {
 public:
  FirestoreWeatherRepository(FirestoreClient &_client, JsonCoder &_jsonCoder, SdCardStorage &_sdCardStorage);

  bool sendWeatherData(WeatherModel weather) override;
  bool sendRainDetected() override;

 private:
  FirestoreClient &client;
  JsonCoder &jsonCoder;
  SdCardStorage &sdCardStorage;

  void cacheWeather(WeatherModel weather);
  void sendCachedWeathers();
  String getFileNameToCacheWeather(String formattedDate);
};

#endif