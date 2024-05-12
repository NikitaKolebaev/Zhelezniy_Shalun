# Zhelezniy_Shalun
## 1. Preview
Arduino based bot with radio control. The challenge we had accepted consisted of 
few reglament instructions. In our project, it was prohibited to use round wheels 
to accelerate the bot. So, our team decided to use tracks instead. We emphasized 
the idea behind the design, which was inspired by the automotive industry. 
You can find out that people living in cold areas, like Sibir, are using some 
special car tracks to navigate through difficult terrain.

<p align="center">
  <img src="https://i.ytimg.com/vi/nLp_I8ktWKY/hq720.jpg?sqp=-oaymwEhCK4FEIIDSFryq4qpAxMIARUAAAAAGAElAADIQj0AgKJD&rs=AOn4CLBO8HUqJ5wY3LJRwCXbi1ZuhyUpBQ" alt="Example of car tracks construction">
  <br>
  <em>Example of car tracks construction</em>
</p>

## 2. Mechanical construction
Below you can see a photo of our robot's assembly, which shows some of the main structural components. 
Construction contains:
* The main body
* DC motor x2
* Wheelbase with gear, 8 rolls x2
* Track x2
* Battery pack power supply
* The crown battery
* Arduino Uno 
* Motor Sheild
* Radio module RF24
* RGB LED matrix
* Servo
* Power supply tumbler

<p align="center">
  <img src="https://i.ytimg.com/vi/nLp_I8ktWKY/hq720.jpg?sqp=-oaymwEhCK4FEIIDSFryq4qpAxMIARUAAAAAGAElAADIQj0AgKJD&rs=AOn4CLBO8HUqJ5wY3LJRwCXbi1ZuhyUpBQ" alt="Example of car tracks construction">
  <br>
  <em>CAD assembly in Compass3D</em>
</p>

<p align="center">
  <img src="https://i.ytimg.com/vi/nLp_I8ktWKY/hq720.jpg?sqp=-oaymwEhCK4FEIIDSFryq4qpAxMIARUAAAAAGAElAADIQj0AgKJD&rs=AOn4CLBO8HUqJ5wY3LJRwCXbi1ZuhyUpBQ" alt="Example of car tracks construction">
  <br>
  <em>Photo of the real bot</em>
</p>

## 3. Controller navigation
To navigate our bot we used to create a special controller. Here you can see a list of all the components we 
have in the construction of the controller:
* Controller body (consists of 2 parts: the top and the bottom)
* Sticks to provide the structural strength
* Arduino Uno
* Radio module RF24
* Power supply tumbler
* The crown battery
* Potentiometer
* LED tumbler
* joystick-module to navigate through the vertiacal axis
* joystick-module to navigate through the horizontal axis

Here you can see the script of controller.

```C++
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9); // CE, CSN
int data[4];  

const int xPin = A1; //0-540-1023
const int yPin = A0; //0-496-1023
//const int buttonPin = 4;
const int ptmrPin = A2; //0-1023
const int tumblrPin = 4;

int x_pos = 0;
int y_pos = 0;
int buttonState = 0;
int tumblrState = 0;
int ptmr = 0;
int for_back_dir = 0;
                                
void setup(){
    radio.begin();
    radio.setChannel(16);
    radio.setDataRate(RF24_1MBPS);  
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(0xABABCC1766LL);
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    //pinMode(buttonPin, INPUT_PULLUP);
    pinMode(tumblrPin, INPUT_PULLUP);
    pinMode(ptmrPin, INPUT);
}
void loop() {
    x_pos = analogRead(xPin);
    y_pos = analogRead(yPin);
    //buttonState = digitalRead(buttonPin);
    tumblrState = digitalRead(tumblrPin);
    ptmr = analogRead(ptmrPin);

    if (y_pos > 540){
      //forward
      for_back_dir = 1;
      //message = "forward";
    }
    else if (y_pos < 450){
      //backward
      for_back_dir = -1;
      //message = "backward";
    }
    else{
      //stop
      //message = "stop";
      for_back_dir = 0;
    }

    data[0] = for_back_dir;
    data[1] = x_pos;
    data[2] = ptmr;
    data[3] = tumblrState;
    //data[4] = buttonState;

    radio.write(&data, sizeof(data));
    delay(10);
}
```

## 4. Bot script
```C++
//#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
//Приемник (робот)

#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

RF24 radio(8, 9); // CE, CSN
Servo servo;
int data[4];
int angle;
// for_back_dir, x_pos, ptmr, tumblrState

void setup() {
  Serial.begin(9600);
  servo.attach(2);

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
```
