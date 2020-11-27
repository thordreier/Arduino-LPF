/*
 * http://brickostan.com/arduino-lpf/
 */




#ifndef Arduino_LPF_common_h
#define Arduino_LPF_common_h




#include "Arduino-LPF-types.h"
#include "Arduino-LPF-values.h"




//LRC check
byte lpf_lrc_check(lpf_msg msg);
lpf_msg lpf_lrc_calc(lpf_msg msg);


//Nibbles
byte lpf_get_nibble1(lpf_msg msg);
byte lpf_get_nibble2(lpf_msg msg);
byte lpf_get_nibble3(lpf_msg msg);
byte lpf_get_nibble4(lpf_msg msg);


//Nibble 1
byte lpf_get_toggle(lpf_msg msg);
byte lpf_get_escape(lpf_msg msg);
byte lpf_get_channel(lpf_msg msg);


//Nibble 2
byte lpf_get_address(lpf_msg msg);
byte lpf_get_mode(lpf_msg msg);


//Nibble 3
//empty


//other
byte lpf_get_brakefloat(lpf_msg msg);
byte lpf_get_output(lpf_msg msg);




#endif
