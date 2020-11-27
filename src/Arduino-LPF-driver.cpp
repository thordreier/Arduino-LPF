/*
 * http://brickostan.com/arduino-lpf/
 */




#include <stdlib.h>
#include "Arduino.h"
#include "Arduino-LPF-driver.h"




LPFDriver::LPFDriver( byte channel, byte address ) {
	set_channel( channel, address );
}




void LPFDriver::set_channel( byte channel, byte address ) {
	_channel = channel;
	_address = address;
}




void LPFDriver::set_checktoggle( byte checktoggle) {
	_checktoggle = checktoggle;
}




void LPFDriver::set_driverfunction( void (*driverfunction) (int8_t speed0, byte brake0, int8_t speed1, byte brake1) ) {
	_driverfunction = driverfunction;
}




void LPFDriver::check_timeout() {
	if( _timeout && millis() > _timeout ) {
		_timeout = 0;
		if( *_driverfunction ) { //if driverfunction is set, then call it
			_driverfunction( 0, 0, 0, 0 );
		}
	}
}




void LPFDriver::parse_msg( lpf_msg msg ) {

	if( _address == lpf_get_address(msg)
		&& _channel == lpf_get_channel(msg)
	) {

		byte toggle = lpf_get_toggle(msg);
		byte brake[2] = {0};
		byte timeout = 0;

		if( lpf_get_escape(msg) ) { //escape mode
			//fix - not implemented
		} else { //not escape mode
			byte mode = lpf_get_mode(msg);
			if( mode & LPF_MODE_SINGLE ) { //single output mode
				if( ( ! _checktoggle) || _toggle != toggle ) {
					byte data = lpf_get_nibble3(msg);
					byte output = mode & LPF_MODE_SINGLE_OUTPUTBITMASK; //red=0 or blue=1

					if( mode & LPF_MODE_SINGLE_CSTID) { //Clear/Set/Toggle/Inc/Dec
						switch( data ) {
							//case B0000: //Toggle full forward (Stop → Fw, Fw → Stop, Bw → Fw)
							//  //fix - not implemented
							//  break;
							//case B0001: //Toggle direction
							//  //fix - not implemented
							//  break;
							//case B0010: //Increment numerical PWM
							//  //fix - not implemented
							//  break;
							//case B0011: //Decrement numerical PWM
							//  //fix - not implemented
							//  break;
							case B0100: //Increment PWM
								if( _speed[output] < LPF_MAX_SPEED ) ++_speed[output];
								break;
							case B0101: //Decrement PWM
								if( _speed[output] > -LPF_MAX_SPEED ) --_speed[output];
								break;
							//case B0110: //Full forward (timeout)
							//  //fix - not implemented
							//  break;
							//case B0111: //Full backward (timeout)
							//  //fix - not implemented
							//  break;
							//case B1000: //Toggle full forward/backward (default forward)
							//  //fix - not implemented
							//  break;
							//case B1001: //Clear C1 (negative logic – C1 high)
							//  //fix - not implemented
							//  break;
							//case B1010: //Set C1 (negative logic – C1 low)
							//  //fix - not implemented
							//  break;
							//case B1011: //Toggle C1
							//  //fix - not implemented
							//  break;
							//case B1100: //Clear C2 (negative logic – C2 high)
							//  //fix - not implemented
							//  break;
							//case B1101: //Set C2 (negative logic – C2 low)
							//  //fix - not implemented
							//  break;
							//case B1110: //Toggle C2
							//  //fix - not implemented
							//  break;
							//case B1111: //Toggle full backward (Stop → Bw, Bw → Stop, Fwd → Bw)
							//  //fix - not implemented
							//  break;
						}
					} else { //pwm
						if( data == LPF_MODE_SINGLE_PWM_FLOAT ) { //float
							_speed[output] = 0;
						} else if( data == LPF_MODE_SINGLE_PWM_BRAKEFLOAT ) { //brake then float
							_speed[output] = 0;
							brake[output] = 1;
						} else {
							//fix - the following is untested
							if( data & LPF_MODE_SINGLE_PWM_BACKWARDX ) { //backward
								_speed[output] = data ^ LPF_MODE_SINGLE_PWM_BACKWARDX;
							} else { //forward
								_speed[output] = -( data ^ LPF_MODE_SINGLE_PWM_FORWARDX ); 
							}
						}
					}
				}
			} else if( mode & LPF_MODE_RESERVED ) { //reserved mode
				//not implemented - it is only reserved from Lego
			} else if( mode & LPF_MODE_COMBO_DIRECT ) { //combo direct mode
				timeout = 1;
				byte combo_msg[] = { ( msg & LPF_MODE_COMBO_DIRECT_OUTPUT0_BITS_FULL ) >> LPF_MODE_COMBO_DIRECT_OUTPUT0_SHIFT_FULL,
								     ( msg & LPF_MODE_COMBO_DIRECT_OUTPUT1_BITS_FULL ) >> LPF_MODE_COMBO_DIRECT_OUTPUT1_SHIFT_FULL };
				for( int i = 0; i < 2; i++ ) {
					switch( combo_msg[i] ) {
						case LPF_MODE_COMBO_DIRECT_FLOAT:
							_speed[i] = 0;
							break;
						case LPF_MODE_COMBO_DIRECT_FORWARD:
							_speed[i] = LPF_MAX_SPEED;
							break;
						case LPF_MODE_COMBO_DIRECT_BACKWARD:
							_speed[i] = -LPF_MAX_SPEED;
							break;
						case LPF_MODE_COMBO_DIRECT_BRAKE:
							_speed[i] = 0;
							brake[i] = 1;
							break;
					}
				}
			} else { //extended mode
				//fix - not implemented
			}          
		}

		if( timeout ) {
			_timeout = millis() + LPF_TIMEOUT;
		} else {
			_timeout = 0;
		}

		if( *_driverfunction ) { //if driverfunction is set, then call it
			//fixme: think about a timer for the timeout functions!!!
			_driverfunction( _speed[0], brake[0], _speed[1], brake[1] );
		}
		_toggle = toggle; //toggle=this run / _toggle=the saved value, used next time the function i run
	}
}
