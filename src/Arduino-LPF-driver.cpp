/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include "Arduino-LPF-driver.h"
#include <Arduino.h>
#include <stdlib.h>

LpfDriver::LpfDriver(byte channel, byte address) {
  setChannel(channel, address);
}

void LpfDriver::setChannel(byte channel, byte address) {
  _channel = channel;
  _address = address;
}

void LpfDriver::setCheckToggle(byte checktoggle) {
  _checktoggle = checktoggle;
}

void LpfDriver::setDriverFunction(void (
    *driverfunction)(int8_t speed0, byte brake0, int8_t speed1, byte brake1)) {
  _driverfunction = driverfunction;
}

void LpfDriver::checkTimeout() {
  if (_timeout && millis() > _timeout) {
    _timeout = 0;
    _speed[0] = _speed[1] = 0;
    if (*_driverfunction) {  // if driverfunction is set, then call it
      _driverfunction(0, 0, 0, 0);
    }
  }
}

void LpfDriver::parseMessage(lpf_msg msg) {
  if (_address == lpfGetAddress(msg) && _channel == lpfGetChannel(msg)) {
    byte toggle = lpfGetToggle(msg);
    byte brake[2] = {0};
    int8_t oldspeed[2];
    memcpy(oldspeed, _speed, 2);
    byte timeout = 0;

    if (lpfGetEscape(msg)) {  // escape mode
      // fix - not implemented
    } else {  // not escape mode
      byte mode = lpfGetMode(msg);
      if (mode & LPF_MODE_SINGLE) {  // single output mode
        if ((!_checktoggle) || _toggle != toggle) {
          byte data = lpfGetNibble3(msg);
          byte output = mode & LPF_MODE_SINGLE_OUTPUTBITMASK;  // red=0 or
                                                               // blue=1

          if (mode & LPF_MODE_SINGLE_CSTID) {  // Clear/Set/Toggle/Inc/Dec
            switch (data) {
              // case 0b0000: //Toggle full forward (Stop → Fw, Fw → Stop, Bw →
              // Fw)
              //  //fix - not implemented
              //  break;
              // case 0b0001: //Toggle direction
              //  //fix - not implemented
              //  break;
              // case 0b0010: //Increment numerical PWM
              //  //fix - not implemented
              //  break;
              // case 0b0011: //Decrement numerical PWM
              //  //fix - not implemented
              //  break;
              case 0b0100:  // Increment PWM
                if (_speed[output] < LPF_MAX_SPEED)
                  ++_speed[output];
                break;
              case 0b0101:  // Decrement PWM
                if (_speed[output] > -LPF_MAX_SPEED)
                  --_speed[output];
                break;
                // case 0b0110: //Full forward (timeout)
                //  //fix - not implemented
                //  break;
                // case 0b0111: //Full backward (timeout)
                //  //fix - not implemented
                //  break;
                // case 0b1000: //Toggle full forward/backward (default forward)
                //  //fix - not implemented
                //  break;
                // case 0b1001: //Clear C1 (negative logic – C1 high)
                //  //fix - not implemented
                //  break;
                // case 0b1010: //Set C1 (negative logic – C1 low)
                //  //fix - not implemented
                //  break;
                // case 0b1011: //Toggle C1
                //  //fix - not implemented
                //  break;
                // case 0b1100: //Clear C2 (negative logic – C2 high)
                //  //fix - not implemented
                //  break;
                // case 0b1101: //Set C2 (negative logic – C2 low)
                //  //fix - not implemented
                //  break;
                // case 0b1110: //Toggle C2
                //  //fix - not implemented
                //  break;
                // case 0b1111: //Toggle full backward (Stop → Bw, Bw → Stop,
                // Fwd → Bw)
                //  //fix - not implemented
                //  break;
            }
          } else {                                    // pwm
            if (data == LPF_MODE_SINGLE_PWM_FLOAT) {  // float
              _speed[output] = 0;
            } else if (data ==
                       LPF_MODE_SINGLE_PWM_BRAKEFLOAT) {  // brake then float
              _speed[output] = 0;
              brake[output] = 1;
            } else {
              // fix - the following is untested
              if (data & LPF_MODE_SINGLE_PWM_BACKWARDX) {  // backward
                _speed[output] = data ^ LPF_MODE_SINGLE_PWM_BACKWARDX;
              } else {  // forward
                _speed[output] = -(data ^ LPF_MODE_SINGLE_PWM_FORWARDX);
              }
            }
          }
        }
      } else if (mode & LPF_MODE_RESERVED) {  // reserved mode
        // not implemented - it is only reserved from Lego
      } else if (mode & LPF_MODE_COMBO_DIRECT) {  // combo direct mode
        byte combo_msg[] = {
            (byte)((msg & LPF_MODE_COMBO_DIRECT_OUTPUT0_BITS_FULL) >>
                   LPF_MODE_COMBO_DIRECT_OUTPUT0_SHIFT_FULL),
            (byte)((msg & LPF_MODE_COMBO_DIRECT_OUTPUT1_BITS_FULL) >>
                   LPF_MODE_COMBO_DIRECT_OUTPUT1_SHIFT_FULL)};
        for (int i = 0; i < 2; i++) {
          switch (combo_msg[i]) {
            case LPF_MODE_COMBO_DIRECT_FLOAT:
              _speed[i] = 0;
              break;
            case LPF_MODE_COMBO_DIRECT_FORWARD:
              timeout = 1;
              _speed[i] = LPF_MAX_SPEED;
              break;
            case LPF_MODE_COMBO_DIRECT_BACKWARD:
              timeout = 1;
              _speed[i] = -LPF_MAX_SPEED;
              break;
            case LPF_MODE_COMBO_DIRECT_BRAKE:
              _speed[i] = 0;
              brake[i] = 1;
              break;
          }
        }
      } else {  // extended mode
                // fix - not implemented
      }
    }

    if (timeout) {
      _timeout = millis() + LPF_TIMEOUT;
    } else {
      _timeout = 0;
    }

    if (_speed[0] != oldspeed[0] || _speed[1] != oldspeed[1]) {
      if (*_driverfunction) {  // if driverfunction is set, then call it
        _driverfunction(_speed[0], brake[0], _speed[1], brake[1]);
      }
    }

    _toggle = toggle;  // toggle=this run / _toggle=the saved value, used next
                       // time the function i run
  }
}
