/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include "Arduino-LPF-common.h"
#include <Arduino.h>
#include <stdlib.h>

// LRC check
byte lpfLrcCheck(lpf_msg msg) {
  return !(LPF_LRC_CHECK_BITS ^
           ((msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL) ^
           ((msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL) ^
           ((msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL) ^
           ((msg & LPF_NIBBLE4_BITS_FULL) >> LPF_NIBBLE4_SHIFT_FULL));
}

lpf_msg lpfLrcCalc(lpf_msg msg) {
  return msg | (LPF_LRC_CHECK_BITS ^
                ((msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL) ^
                ((msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL) ^
                ((msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL));
}

// Nibbles
byte lpfGetNibble1(lpf_msg msg) {
  return (msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL;
}
byte lpfGetNibble2(lpf_msg msg) {
  return (msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL;
}
byte lpfGetNibble3(lpf_msg msg) {
  return (msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL;
}
byte lpfGetNibble4(lpf_msg msg) {
  return (msg & LPF_NIBBLE4_BITS_FULL) >> LPF_NIBBLE4_SHIFT_FULL;
}

// Nibble 1
byte lpfGetToggle(lpf_msg msg) {
  return (msg & LPF_TOGGLE_BITS_FULL) >> LPF_TOGGLE_SHIFT_FULL;
}
byte lpfGetEscape(lpf_msg msg) {
  return (msg & LPF_ESCAPE_BITS_FULL) >> LPF_ESCAPE_SHIFT_FULL;
}
byte lpfGetChannel(lpf_msg msg) {
  return (msg & LPF_CHANNEL_BITS_FULL) >> LPF_CHANNEL_SHIFT_FULL;
}

// Nibble 2
byte lpfGetAddress(lpf_msg msg) {
  return (msg & LPF_ADDRESS_BITS_FULL) >> LPF_ADDRESS_SHIFT_FULL;
}
byte lpfGetMode(lpf_msg msg) {
  return (msg & LPF_MODE_BITS_FULL) >> LPF_MODE_SHIFT_FULL;
}

// Nibble 3
byte lpfGetBrakefloat(lpf_msg msg) {
  return !((msg & LPF_MODE_SINGLE_PWM_BRAKEFLOAT_FULL_LOOKAT) ^
           LPF_MODE_SINGLE_PWM_BRAKEFLOAT_FULL_VALUES);
}
byte lpfGetOutput(lpf_msg msg) {
  return ((msg & LPF_MODE_SINGLE_OUTPUT_FULL_LOOKAT) &
          LPF_MODE_SINGLE_OUTPUT_FULL_VALUES) >>
         LPF_MODE_SINGLE_OUTPUT_FULL_SHIFT;
}

// Convert LPF speed (-7 to +7) info values that can be used with analogWrite()
uint16_t lpfConvertSpeed(int8_t speed) {
  uint8_t pwmVal = abs(speed);
  if (pwmVal > 7)  // just sanitizing data
    pwmVal = 7;
  pwmVal = pwmVal << 5 | pwmVal << 2 | pwmVal >> 1;
  if (speed > 0)
    return pwmVal;
  else
    return pwmVal << 8;
}

// Set motor speed based on LPF speed (-7 to +7)
uint16_t lpfSetMotorSpeed(byte pin1,
                          byte pin2,
                          int8_t speed,
                          void (*pwm_function)(uint8_t pin, int val)) {
  uint16_t pwmSpeed = lpfConvertSpeed(speed);
  pwm_function(pin1, (uint8_t)(pwmSpeed));
  pwm_function(pin2, (uint8_t)(pwmSpeed >> 8));
  return pwmSpeed;
}
