//#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
//Приемник (робот)

#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

#define MATRIX_PIN   0
#define NUMPIXELS 7

RF24 radio(8, 9); // CE, CSN
Servo servo;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);
int data[4];    
int angle;
// for_back_dir, x_pos, ptmr, tumblrState

void setup() {
  Serial.begin(9600);
  servo.attach(2);
  pixels.begin();

  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }

  radio.begin();
  radio.setChannel(16);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(0, 0xABABCC1766LL);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));
    angle = map(data[1], 0, 1023, 45, 135);
    servo.write(angle);
    //Serial.println(servo.read());

    if (data[0] == 1){
      digitalWrite(DIR_1, HIGH);
      digitalWrite(DIR_2, HIGH);
      if (angle < 60){
        analogWrite(SPEED_2, data[2] - (data[2] / 3));
        analogWrite(SPEED_1, data[2]);
      }
      else if (angle > 120){
        analogWrite(SPEED_2, data[2]);
        analogWrite(SPEED_1, data[2] - (data[2] / 3));
      }
      else{
        analogWrite(SPEED_1, data[2]);
        analogWrite(SPEED_2, data[2]);
      }
    }
    else if (data[0] == -1){
      digitalWrite(DIR_1, LOW);
      digitalWrite(DIR_2, LOW);
      if (angle < 60){
        analogWrite(SPEED_2, data[2] - (data[2] / 3));
        analogWrite(SPEED_1, data[2]);
      }
      else if (angle > 120) {
        analogWrite(SPEED_2, data[2]);
        analogWrite(SPEED_1, data[2] - (data[2] / 3));
      }
      else{
        analogWrite(SPEED_1, data[2]);
        analogWrite(SPEED_2, data[2]);
      }
    }
    else {
      analogWrite(SPEED_1, 0);
      analogWrite(SPEED_2, 0);
    }
    //print_info(data[0], angle, data[2], data[3]);

    if (data[3] == 1){
        for(int i=0; i<NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 150, 0));
        }
        pixels.show();
    }
    else{
        for(int i=0; i<NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
        pixels.show();
    }
    delay(2);
  }
}
/*
void print_info(int y, int angle, int ptmr, int tmbl) {
    Serial.print("y = ");
    Serial.print(y);
    Serial.print(", angle = ");
    Serial.print(angle);
    Serial.print(", ptmr = ");
    Serial.print(ptmr);
    Serial.print(", tmbl = ");
    Serial.println(tmbl);
}*/
