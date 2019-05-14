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
  while( lpf_msg msg = lpf_receiver_get_next_msg() )
  {
    Serial.println( msg, BIN );
  }
}
