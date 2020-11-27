/*
 * http://brickostan.com/arduino-lpf/
 */




#ifndef Arduino_LPF_driver_h
#define Arduino_LPF_driver_h




#include "Arduino-LPF-types.h"
#include "Arduino-LPF-values.h"
#include "Arduino-LPF-common.h"




class LPFDriver {
	private:
		byte _channel;
		byte _address;
		byte _checktoggle = 1;
		byte _toggle = 0;
		int8_t _speed[2] = {0};
		void (*_driverfunction) (int8_t speed0, byte brake0, int8_t speed1, byte brake1) = NULL;
	public:
		LPFDriver( byte channel, byte address = 0);
		void set_channel( byte channel, byte address = 0);
		void set_checktoggle( byte checktoggle);
		void set_driverfunction( void (*driverfunction) (int8_t speed0, byte brake0, int8_t speed1, byte brake1) );
		void parse_msg( lpf_msg msg );
};




#endif
