#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//Приемник (робот)
RF24 radio(8, 9); // CE, CSN
int data[2];

void setup() {
  Serial.begin(9600);
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
    Serial.print(data[0]);
    Serial.print(" ");
    Serial.println(data[1]);
  }
}
