/*
 * http://brickostan.com/arduino-lpf/
 */




#include <stdlib.h>
#include "Arduino.h"
#include "Arduino-LPF-receiver.h"




static volatile byte _pin = 0;
static volatile byte _sanitize = 0;
static volatile lpf_msg _LPF_MSG_QUEUE[LPF_QUEUE_SIZE] = {0};




void lpf_receiver_set_pin( byte pin ) {
	_pin = pin;
}


void lpf_receiver_set_sanitize( byte sanitize ) {
	_sanitize = sanitize;
}




lpf_msg lpf_receiver_get_next_msg() {
	static byte queue_ptr = 0;
	lpf_msg msg = 0;
	if( _LPF_MSG_QUEUE[queue_ptr] ) {
		//fixme: maybe the next two lines should be run with interrupts disabled
		msg = _LPF_MSG_QUEUE[queue_ptr];
		_LPF_MSG_QUEUE[queue_ptr] = 0;
		queue_ptr = ( queue_ptr + 1 ) % LPF_QUEUE_SIZE;
		if(! lpf_lrc_check( msg ) ) {
			msg = 0;
		}
	}
	return msg;
}




//fixme: the last message is only found when the next IR pulse is received
//fixme: message is received regardless of start/stop bit or not
void lpf_receiver_interrupt() {

	static unsigned long time_prev             = 0;
	static unsigned long time_diff             = 0;
	static byte          irvalue_prev          = 0;
	static byte          transmitting_bitcount = 0;
	static lpf_msg       transmitting_msg      = 0;
	static byte          queue_ptr             = LPF_QUEUE_SIZE - 1;

	// get time and pin value
	byte          irvalue_new = _sanitize ? digitalRead(_pin) : !digitalRead(_pin); //fixme: i don't know if I should remove this sanitize stuff
	unsigned long time_new    = micros();

	if( irvalue_prev != irvalue_new ) { //IR signal has changed
		if( irvalue_new ) { //IR signal has changed to from on to off
			// time between last change (from off to on) and this change (from on to off) - we don't check any time here - it should be arount 160 micro seconds
			time_diff = time_new - time_prev;
		} else { //IR signal has changed from off to on
			// time between last time it change from off to on to this time it changed from off to on - a full PF bit
			time_diff = time_new - time_prev + time_diff;
			
			//fixme: this stuff should also be checked when there is more than 947us silence
			if( transmitting_bitcount == LPF_MSG_BITCOUNT && time_new - time_prev + time_diff > 947 ) {
				transmitting_bitcount = 0;
				queue_ptr = ( queue_ptr + 1 ) % LPF_QUEUE_SIZE;
				_LPF_MSG_QUEUE[queue_ptr] = transmitting_msg;
			}

			// find out if it's low bit (0), high bit (1) or start stop bit
			if( time_diff > LPF_IR_LOW_MIN && time_diff < LPF_IR_LOW_MAX ) { // low bit range (0)
				transmitting_bitcount++;
				transmitting_msg = transmitting_msg << 1;
			} else if( time_diff > LPF_IR_HIGH_MIN && time_diff < LPF_IR_HIGH_MAX ) { //high bit range (1)
				transmitting_bitcount++;
				transmitting_msg = (transmitting_msg << 1) | B1;
			} else if( time_diff > LPF_IR_STARTSTOP_MIN && time_diff < LPF_IR_STARTSTOP_MAX ) { //start stop bit range
				transmitting_bitcount = 0;
				transmitting_msg      = 0;
			} else {
				//fixme: reset some counters
			}
		}

		// set "previous" values - used next thime the function is run
		time_prev    = time_new;
		irvalue_prev = irvalue_new;
	}
}
