#ifndef CONTINUOUS_SENSOR_READER_H
#define CONTINUOUS_SENSOR_READER_H

template <typename T>
class ContinuousSensorReader {
 public:
  virtual bool begin();
  virtual void update();
  virtual void startReading();
  virtual void stopReading();
  virtual T getData();
};

#endif