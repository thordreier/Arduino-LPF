/*
 * http://brickostan.com/arduino-lpf/
 */

#include "Arduino-LPF-receiver.h"

const byte IRreceiverPin = 2;

void setup() {
  Serial.begin( 9600 );

  pinMode( IRreceiverPin, INPUT );
  lpf_receiver_set_pin( IRreceiverPin );
  attachInterrupt( digitalPinToInterrupt( IRreceiverPin ), lpf_receiver_interrupt, CHANGE );
}

void loop() {
  while( lpf_msg msg = lpf_receiver_get_next_msg() ) {

    // If using  this, then leading zeroes will not be printed
    //Serial.println( msg, BIN );

    // Using this will always print all 16 bits
    for( int i = 15; i >= 0; i--) {
      Serial.print((msg >> i) & 1 == 1 ? "1" : "0");
    }
    Serial.println();

  }
}
