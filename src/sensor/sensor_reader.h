#ifndef SENSOR_READER_H
#define SENSOR_READER_H

template <typename T>
class SensorReader {
 public:
  virtual bool begin();
  virtual T read();
};

#endif