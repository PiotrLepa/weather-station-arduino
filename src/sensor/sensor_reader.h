#ifndef SENSOR_READER_H
#define SENSOR_READER_H

template <typename T>
class SensorReader {
 public:
  virtual void begin();
  virtual bool read();
  virtual T getData();
  virtual String getErrorMessage();
};

#endif