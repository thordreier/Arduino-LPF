/*
 * https://github.com/thordreier/Arduino-LPF
 */

#ifndef Arduino_LPF_common_h
#define Arduino_LPF_common_h

#include <stdlib.h>
#include "Arduino-LPF-types.h"
#include "Arduino-LPF-values.h"
#include "Arduino.h"

// LRC check
byte lpf_lrc_check(lpf_msg msg);
lpf_msg lpf_lrc_calc(lpf_msg msg);

// Nibbles
byte lpf_get_nibble1(lpf_msg msg);
byte lpf_get_nibble2(lpf_msg msg);
byte lpf_get_nibble3(lpf_msg msg);
byte lpf_get_nibble4(lpf_msg msg);

// Nibble 1
byte lpf_get_toggle(lpf_msg msg);
byte lpf_get_escape(lpf_msg msg);
byte lpf_get_channel(lpf_msg msg);

// Nibble 2
byte lpf_get_address(lpf_msg msg);
byte lpf_get_mode(lpf_msg msg);

// Nibble 3
byte lpf_get_brakefloat(lpf_msg msg);  // only works on single mode/PWM
byte lpf_get_output(lpf_msg msg);      // only works on single mode

// other
byte lpf_get_brakefloat(lpf_msg msg);
byte lpf_get_output(lpf_msg msg);

// Convert LPF speed (-7 to +7) info values that can be used with analogWrite
uint16_t lpf_convert_speed(int8_t speed);

// Set motor speed based on LPF speed (-7 to +7)
uint16_t lpf_set_motor_speed(byte pin1,
                             byte pin2,
                             int8_t speed,
                             void (*pwm_function)(uint8_t pin,
                                                  int val) = analogWrite);

#endif
