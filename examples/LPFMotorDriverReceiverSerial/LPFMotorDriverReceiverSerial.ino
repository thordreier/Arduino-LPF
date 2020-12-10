/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include <Arduino-LPF-driver.h>
#include <Arduino-LPF-receiver.h>

const byte ir_pin = 2;
byte channel = 3;  // between 0 and 3 (for channel 1-4)

LPFDriver driver(channel);

void control_motor(int8_t speed0, byte brake0, int8_t speed1, byte brake1) {
  Serial.print("Red speed: ");
  Serial.print(speed0 > 0 ? "+" : speed0 ? "" : " ");
  Serial.print(speed0);
  Serial.print(" / Blue speed: ");
  Serial.print(speed1 > 0 ? "+" : speed1 ? "" : " ");
  Serial.print(speed1);
  Serial.println();
}

void setup() {
  Serial.begin(9600);

  pinMode(ir_pin, INPUT);
  lpfReceiverSetPin(ir_pin);
  attachInterrupt(digitalPinToInterrupt(ir_pin),
                  lpfReceiverInterruptHandler, CHANGE);
  driver.setDriverFunction(control_motor);
}

void loop() {
  driver.checkTimeout();
  while (lpf_msg msg = lpfReceiverGetNextMessage()) {
    driver.parseMessage(msg);
  }
}
