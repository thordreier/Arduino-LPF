/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include "Arduino-LPF-common.h"
#include <stdlib.h>
#include "Arduino.h"

// LRC check
byte lpf_lrc_check(lpf_msg msg) {
  return !(LPF_LRC_CHECK_BITS ^
           ((msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL) ^
           ((msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL) ^
           ((msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL) ^
           ((msg & LPF_NIBBLE4_BITS_FULL) >> LPF_NIBBLE4_SHIFT_FULL));
}

lpf_msg lpf_lrc_calc(lpf_msg msg) {
  return msg | (LPF_LRC_CHECK_BITS ^
                ((msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL) ^
                ((msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL) ^
                ((msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL));
}

// Nibbles
byte lpf_get_nibble1(lpf_msg msg) {
  return (msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL;
}
byte lpf_get_nibble2(lpf_msg msg) {
  return (msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL;
}
byte lpf_get_nibble3(lpf_msg msg) {
  return (msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL;
}
byte lpf_get_nibble4(lpf_msg msg) {
  return (msg & LPF_NIBBLE4_BITS_FULL) >> LPF_NIBBLE4_SHIFT_FULL;
}

// Nibble 1
byte lpf_get_toggle(lpf_msg msg) {
  return (msg & LPF_TOGGLE_BITS_FULL) >> LPF_TOGGLE_SHIFT_FULL;
}
byte lpf_get_escape(lpf_msg msg) {
  return (msg & LPF_ESCAPE_BITS_FULL) >> LPF_ESCAPE_SHIFT_FULL;
}
byte lpf_get_channel(lpf_msg msg) {
  return (msg & LPF_CHANNEL_BITS_FULL) >> LPF_CHANNEL_SHIFT_FULL;
}

// Nibble 2
byte lpf_get_address(lpf_msg msg) {
  return (msg & LPF_ADDRESS_BITS_FULL) >> LPF_ADDRESS_SHIFT_FULL;
}
byte lpf_get_mode(lpf_msg msg) {
  return (msg & LPF_MODE_BITS_FULL) >> LPF_MODE_SHIFT_FULL;
}

// Nibble 3
byte lpf_get_brakefloat(lpf_msg msg) {
  return !((msg & LPF_MODE_SINGLE_PWM_BRAKEFLOAT_FULL_LOOKAT) ^
           LPF_MODE_SINGLE_PWM_BRAKEFLOAT_FULL_VALUES);
}
byte lpf_get_output(lpf_msg msg) {
  return ((msg & LPF_MODE_SINGLE_OUTPUT_FULL_LOOKAT) &
          LPF_MODE_SINGLE_OUTPUT_FULL_VALUES) >>
         LPF_MODE_SINGLE_OUTPUT_FULL_SHIFT;
}

// Convert LPF speed (-7 to +7) info values that can be used with analogWrite()
uint16_t lpf_convert_speed(int8_t speed) {
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
uint16_t lpf_set_motor_speed(byte pin1,
                             byte pin2,
                             int8_t speed,
                             void (*pwm_function)(uint8_t pin, int val)) {
  uint16_t pwmSpeed = lpf_convert_speed(speed);
  pwm_function(pin1, (uint8_t)(pwmSpeed));
  pwm_function(pin2, (uint8_t)(pwmSpeed >> 8));
  return pwmSpeed;
}
