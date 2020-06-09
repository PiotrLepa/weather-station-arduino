#ifndef SEONSOR_CONTROLLER_H
#define SEONSOR_CONTROLLER_H

template <typename T>
class SensorController {
 public:
  virtual void begin();
  virtual bool read();
  virtual T getData();
  virtual String getErrorMessage();
};

#endif