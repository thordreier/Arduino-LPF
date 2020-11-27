/*
 * http://brickostan.com/arduino-lpf/
 */




#ifndef Arduino_LPF_receiver_h
#define Arduino_LPF_receiver_h




#include "Arduino-LPF-types.h"
#include "Arduino-LPF-values.h"
#include "Arduino-LPF-common.h"




#define LPF_QUEUE_SIZE 8




void    lpf_receiver_set_pin      (byte);
void    lpf_receiver_set_sanitize (byte);
lpf_msg lpf_receiver_get_next_msg ();
void    lpf_receiver_interrupt    ();




#endif
