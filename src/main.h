
#ifndef MAIN_H

#include <Arduino.h>

#include "config/config.h"
#include "config/private_config.h"
#include "json_encoder/json_encoder.h"
#include "model/air_quality/air_quality_model.h"
#include "model/weather/weather_model.h"
#include "pm_controller/pm_controller.h"
#include "rest_client/rest_client.h"
#include "weather_repository/weather_repository.h"

void setup();
void loop();

#endif