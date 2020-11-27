/*
 * http://brickostan.com/arduino-lpf/
 */




#include <stdlib.h>
#include "Arduino.h"
#include "Arduino-LPF-common.h"




byte lpf_lrc_check(lpf_msg msg) {
	return
		! ( LPF_LRC_CHECK_BITS
		^ ( (msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL )
		^ ( (msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL )
		^ ( (msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL )
		^ ( (msg & LPF_NIBBLE4_BITS_FULL) >> LPF_NIBBLE4_SHIFT_FULL )
		);
}




lpf_msg lpf_lrc_calc(lpf_msg msg) {
	return
		msg | ( LPF_LRC_CHECK_BITS
		^ ( (msg & LPF_NIBBLE1_BITS_FULL) >> LPF_NIBBLE1_SHIFT_FULL )
		^ ( (msg & LPF_NIBBLE2_BITS_FULL) >> LPF_NIBBLE2_SHIFT_FULL )
		^ ( (msg & LPF_NIBBLE3_BITS_FULL) >> LPF_NIBBLE3_SHIFT_FULL )
		);
}




//Nibbles
byte lpf_get_nibble1(lpf_msg msg)  { return ( msg & LPF_NIBBLE1_BITS_FULL  ) >> LPF_NIBBLE1_SHIFT_FULL  ; }
byte lpf_get_nibble2(lpf_msg msg)  { return ( msg & LPF_NIBBLE2_BITS_FULL  ) >> LPF_NIBBLE2_SHIFT_FULL  ; }
byte lpf_get_nibble3(lpf_msg msg)  { return ( msg & LPF_NIBBLE3_BITS_FULL  ) >> LPF_NIBBLE3_SHIFT_FULL  ; }
byte lpf_get_nibble4(lpf_msg msg)  { return ( msg & LPF_NIBBLE4_BITS_FULL  ) >> LPF_NIBBLE4_SHIFT_FULL  ; }




//Nibble 1
byte lpf_get_toggle(lpf_msg msg)  { return ( msg & LPF_TOGGLE_BITS_FULL  ) >> LPF_TOGGLE_SHIFT_FULL  ; }
byte lpf_get_escape(lpf_msg msg)  { return ( msg & LPF_ESCAPE_BITS_FULL  ) >> LPF_ESCAPE_SHIFT_FULL  ; }
byte lpf_get_channel(lpf_msg msg) { return ( msg & LPF_CHANNEL_BITS_FULL ) >> LPF_CHANNEL_SHIFT_FULL ; }




//Nibble 2
byte lpf_get_address(lpf_msg msg) { return ( msg & LPF_ADDRESS_BITS_FULL ) >> LPF_ADDRESS_SHIFT_FULL ; }
byte lpf_get_mode(lpf_msg msg)    { return ( msg & LPF_MODE_BITS_FULL    ) >> LPF_MODE_SHIFT_FULL    ; }




//Nibble 3
//empty




//only works on single mode/pwm
byte lpf_get_brakefloat(lpf_msg msg) {
	return ! ( ( msg & LPF_MODE_SINGLE_PWM_BRAKEFLOAT_FULL_LOOKAT ) ^ LPF_MODE_SINGLE_PWM_BRAKEFLOAT_FULL_VALUES );
}




//only works on single mode
byte lpf_get_output(lpf_msg msg) {
	return ( ( msg & LPF_MODE_SINGLE_OUTPUT_FULL_LOOKAT ) & LPF_MODE_SINGLE_OUTPUT_FULL_VALUES ) >> LPF_MODE_SINGLE_OUTPUT_FULL_SHIFT;
}
