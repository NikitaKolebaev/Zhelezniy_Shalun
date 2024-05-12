#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//Передатчик (пульт)
RF24 radio(8, 9); // CE, CSN
int data[2];

void setup() {
  radio.begin();
  radio.setChannel(16);
  radio.setDataRate(RF24_1MBPS);  
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(0xABABCC1766LL);
}
void loop() {
  data[0] = 0;
  data[1] = 1;

  radio.write(&data, sizeof(data));
  delay(500);
}
