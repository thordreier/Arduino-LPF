#include "Arduino-LPF-receiver.h"
#include "Arduino-LPF-driver.h"

const byte IRreceiverPin = 2;
byte       channel       = 3; //between 0 and 3 (for channel 1-4)

LPFDriver driver( channel);

void control_motor( int8_t speed0, byte brake0, int8_t speed1, byte brake1 ) {
  Serial.print( "Red speed: " );
  Serial.print( speed0 > 0 ? "+" : speed0 ? "" : " ");
  Serial.print( speed0 );
  Serial.print( " / Blue speed: " );
  Serial.print( speed1 > 0 ? "+" : speed1 ? "" : " ");
  Serial.print( speed1 );
  Serial.println();
}

void setup() {
  Serial.begin( 9600 );

  pinMode(IRreceiverPin, INPUT);
  lpf_receiver_set_pin(IRreceiverPin);
  attachInterrupt(digitalPinToInterrupt(IRreceiverPin), lpf_receiver_interrupt, CHANGE);
  driver.set_driverfunction( control_motor );
}

void loop() {
  driver.check_timeout();
  while( lpf_msg msg = lpf_receiver_get_next_msg() ) {
    driver.parse_msg(msg);
  }
}
