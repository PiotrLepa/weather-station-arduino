#include "main.h"

RestClient restClient = RestClient(API_URL);
JsonEncoder jsonEncoder = JsonEncoder();
WeatherRepository weatherRepository =
    WeatherRepository(restClient, jsonEncoder);

AirQualityReader airQualityReader = AirQualityReader(Serial);
TemperatureReader tempReader = TemperatureReader(TEMPERATURE_SENSOR_PIN);
WindReader windReader = WindReader(WIND_SENSOR_PIN);
RainGaugeReader rainGaugeReader = RainGaugeReader(RAIN_GAUGE_SENSOR_PIN);

Ticker serverRequestTimer = Ticker(gatherWeatherData, 15000);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  restClient.connectToWifi(WIFI_SSID, WIFI_PASSWORD);

  serverRequestTimer.start();

  begin();
  startSensors();
}

void loop() {
  serverRequestTimer.update();
  windReader.update();
}

void begin() {
  tempReader.begin();
  airQualityReader.begin();
  windReader.begin();
  rainGaugeReader.begin();
}

void startSensors() {
  windReader.startReading();
  rainGaugeReader.startReading();
}

void gatherWeatherData() {
  windReader.stopReading();
  rainGaugeReader.stopReading();

  TemperatureModel temperatureModel;
  if (tempReader.read()) {
    temperatureModel = tempReader.getData();
  } else {
    Serial.println(tempReader.getErrorMessage());
  }

  AirQualityModel airQualityModel;
  if (airQualityReader.read()) {
    airQualityModel = airQualityReader.getData();
  } else {
    Serial.println(airQualityReader.getErrorMessage());
  }

  WindModel windModel = windReader.getData();
  RainGaugeModel rainGaugeModel = rainGaugeReader.getData();

  sendWeatherDataToServer(temperatureModel, airQualityModel, windModel,
                          rainGaugeModel);

  startSensors();
}

void sendWeatherDataToServer(TemperatureModel temperature,
                             AirQualityModel airQuality, WindModel wind,
                             RainGaugeModel rainGauge) {
  WeatherModel model = WeatherModel(temperature, airQuality, wind, rainGauge);
  weatherRepository.sendWeatherData(model);
}