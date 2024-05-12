#include <Servo.h>

Servo servo;
const int serv = 2;

void setup() {
  servo.attach(serv);
  servo.write(0);
  delay(1000);
}

void loop() {
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(0);
  delay(1000);
  //out = map(ptmr, 0, 1023, 0, 180);
}
