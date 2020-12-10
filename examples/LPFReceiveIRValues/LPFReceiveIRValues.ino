/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include <Arduino-LPF-receiver.h>

const byte ir_pin = 2;

void setup() {
  Serial.begin(9600);

  pinMode(ir_pin, INPUT);
  lpfReceiverSetPin(ir_pin);
  attachInterrupt(digitalPinToInterrupt(ir_pin),
                  lpfReceiverInterruptHandler, CHANGE);
}

void loop() {
  while (lpf_msg msg = lpfReceiverGetNextMessage()) {
    // If using  this, then leading zeroes will not be printed
    // Serial.println( msg, BIN );

    // Using this will always print all 16 bits
    for (int i = 15; i >= 0; i--) {
      Serial.print((msg >> i) & 1 == 1 ? "1" : "0");
    }
    Serial.println();
  }
}
