#ifndef PERIODIC_SENSOR_READER_H
#define PERIODIC_SENSOR_READER_H

template <typename T>
class PeriodicSensorReader {
 public:
  virtual void begin();
  virtual bool startReading();
  virtual bool stopReading();
  virtual T getData();
  virtual String getErrorMessage();
};

#endif