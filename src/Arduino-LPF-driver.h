/*
 * https://github.com/thordreier/Arduino-LPF
 */

#ifndef Arduino_LPF_driver_h
#define Arduino_LPF_driver_h

#include <Arduino.h>
#include <stdlib.h>
#include "Arduino-LPF-common.h"
#include "Arduino-LPF-types.h"
#include "Arduino-LPF-values.h"

class LpfDriver {
 private:
  byte _channel;
  byte _address;
  byte _checktoggle = 1;
  byte _toggle = 0;
  int8_t _speed[2] = {0};
  unsigned long _timeout = 0;
  void (*_driverfunction)(int8_t speed0,
                          byte brake0,
                          int8_t speed1,
                          byte brake1) = NULL;

 public:
  LpfDriver(byte channel, byte address = 0);
  void setChannel(byte channel, byte address = 0);
  void setCheckToggle(byte checktoggle);
  void setDriverFunction(void (
      *driverfunction)(int8_t speed0, byte brake0, int8_t speed1, byte brake1));
  void checkTimeout();
  void parseMessage(lpf_msg msg);
};

#endif  // Arduino_LPF_driver_h
