/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include <Arduino-LPF-driver.h>
#include <Arduino-LPF-receiver.h>

const byte ir_pin = 2;
byte channel = 3;  // between 0 and 3 (for channel 1-4)

LpfDriver driver(channel);

void printNumber(uint8_t i) {
  Serial.print(i < 10 ? "  " : i < 100 ? " " : "");
  Serial.print(i);
}

void printSpeed(int8_t speed) {
  uint16_t pwm = lpfConvertSpeed(speed);
  Serial.print(speed > 0 ? "+" : speed ? "" : " ");
  Serial.print(speed);
  Serial.print(" PWM(");
  printNumber((uint8_t)(pwm >> 8));
  Serial.print(",");
  printNumber((uint8_t)(pwm));
  Serial.print(")");
}

void controlMotor(int8_t speed0, byte brake0, int8_t speed1, byte brake1) {
  Serial.print("Red speed: ");
  printSpeed(speed0);
  Serial.print(" / Blue speed: ");
  printSpeed(speed1);
  Serial.println();
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
