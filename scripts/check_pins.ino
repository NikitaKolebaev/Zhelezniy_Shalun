#include <SPI.h>

const int xPin = A1; //0-540-1023
const int yPin = A0; //0-496-1023
//const int buttonPin = 4;
const int ptmrPin = A2; //0-1023
const int tumblrPin = 4;


void setup() {
  Serial.begin(9600);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  //pinMode(buttonPin, INPUT_PULLUP);
  pinMode(tumblrPin, INPUT);
  pinMode(ptmrPin, INPUT);
}

void loop() {
  int x_value = analogRead(xPin);
  int y_value = analogRead(yPin);
  int tumblState = digitalRead(tumblrPin);
  int ptmr = analogRead(ptmrPin); 
  Serial.print("x = ");
  Serial.print(x_value);
  Serial.print(", y = ");
  Serial.print(y_value);
  Serial.print(", bt = ");
  Serial.print(tumblState);
  Serial.print(", ptmr = ");
  Serial.println(ptmr);
}
