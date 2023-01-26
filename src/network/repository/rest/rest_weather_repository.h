#ifndef REST_WEATHER_REPOSITORY_H
#define REST_WEATHER_REPOSITORY_H

#include <HTTPClient.h>

#include "../../../model/weather/weather_model.h"
#include "../../../storage/sd_card/sd_card_storage.h"
#include "../../../utils/date_time/date_time.h"
#include "../../../utils/json_coder/json_coder.h"
#include "../../client/rest/rest_client.h"
#include "../weather_repository.h"

class RestWeatherRepository : public WeatherRepository {
   public:
    RestWeatherRepository(RestClient &_client, JsonCoder &_jsonCoder, SdCardStorage &_sdCardStorage);

    bool sendWeatherData(WeatherModel weather) override;
    bool sendRainDetected() override;

   private:
    RestClient &client;
    JsonCoder &jsonCoder;
    SdCardStorage &sdCardStorage;

    void cacheWeather(WeatherModel weather);
    void sendCachedWeathers();
    String getFileNameToCacheWeather(String formattedDate);
};

#endif