#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

struct WeatherModel {
  int temperature;

  WeatherModel(int _temperature) : temperature(_temperature) {}
};

#endif