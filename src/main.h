
#ifndef MAIN_H

#include <Arduino.h>

#include "config.h"
#include "json_encoder/json_encoder.h"
#include "model/weather_model.h"
#include "private_config.h"
#include "rest_client/rest_client.h"
#include "weather_repository/weather_repository.h"

void setup();
void loop();

#endif