/*
 * https://github.com/thordreier/Arduino-LPF
 */

#ifndef Arduino_LPF_common_h
#define Arduino_LPF_common_h

#include <Arduino.h>
#include <stdlib.h>
#include "Arduino-LPF-types.h"
#include "Arduino-LPF-values.h"

// LRC check
byte lpfLrcCheck(lpf_msg msg);
lpf_msg lpfLrcCalc(lpf_msg msg);

// Nibbles
byte lpfGetNibble1(lpf_msg msg);
byte lpfGetNibble2(lpf_msg msg);
byte lpfGetNibble3(lpf_msg msg);
byte lpfGetNibble4(lpf_msg msg);

// Nibble 1
byte lpfGetToggle(lpf_msg msg);
byte lpfGetEscape(lpf_msg msg);
byte lpfGetChannel(lpf_msg msg);

// Nibble 2
byte lpfGetAddress(lpf_msg msg);
byte lpfGetMode(lpf_msg msg);

// Nibble 3
byte lpfGetBrakeFloat(lpf_msg msg);  // only works on single mode/PWM
byte lpfGetOutput(lpf_msg msg);      // only works on single mode

// Convert LPF speed (-7 to +7) info values that can be used with analogWrite
uint16_t lpfConvertSpeed(int8_t speed);

// Set motor speed based on LPF speed (-7 to +7)
uint16_t lpfSetMotorSpeed(byte pin1,
                          byte pin2,
                          int8_t speed,
                          void (*pwm_function)(uint8_t pin,
                                               int val) = analogWrite);

#endif  // Arduino_LPF_common_h
