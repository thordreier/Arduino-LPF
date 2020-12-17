/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include <Arduino-LPF-driver.h>
#include <Arduino-LPF-receiver.h>

const byte ir_pin = 2;
byte channel = 3;  // between 0 and 3 (for channel 1-4)

LpfDriver driver(channel);

void controlMotor(int8_t speed0, byte brake0, int8_t speed1, byte brake1) {
  char str[255];
  sprintf(
      str,
      "Red speed: %+2i PWM(%3i,%3i) %s / Blue speed: %+2i PWM(%3i,%3i) %s\n",
      speed0, (uint8_t)(lpfConvertSpeed(speed0) >> 8),
      (uint8_t)(lpfConvertSpeed(speed0)), brake0 ? "B" : " ", speed1,
      (uint8_t)(lpfConvertSpeed(speed1) >> 8),
      (uint8_t)(lpfConvertSpeed(speed1)), brake1 ? "B" : " ");
  Serial.print(str);
}

void setup() {
  Serial.begin(9600);

  pinMode(ir_pin, INPUT);
  lpfReceiverSetPin(ir_pin);
  attachInterrupt(digitalPinToInterrupt(ir_pin), lpfReceiverInterruptHandler,
                  CHANGE);
  driver.setDriverFunction(controlMotor);
}

void loop() {
  driver.checkTimeout();
  while (lpf_msg msg = lpfReceiverGetNextMessage()) {
    driver.parseMessage(msg);
  }
}
