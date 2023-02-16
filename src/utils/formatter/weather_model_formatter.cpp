#include "weather_model_formatter.h"

String WeatherModelFormatter::format(WeatherModel weather) {
  return (String) "\nTemperature: " + weather.externalTemperature.temperature + "\nHumidity: " + weather.pressure.humidity +
                 "\nPressure: " + weather.pressure.pressure + "\npm1: " + weather.airQuality.pm1 + "\npm2.5: " + weather.airQuality.pm1 +
                 "\npm25: " + weather.airQuality.pm10 + "\nwindSpeedMax: " + weather.wind.windSpeedMax +
                 "\nwindSpeedAvg: " + weather.wind.windSpeedAvg + "\nprecipitation: " + weather.rainGauge.precipitation +
                 "\ntimestamp: " + weather.timestamp.getFormattedDateTime() + "\n";
}