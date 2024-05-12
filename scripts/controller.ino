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
