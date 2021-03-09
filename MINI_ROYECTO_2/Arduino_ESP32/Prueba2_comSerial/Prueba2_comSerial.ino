
#include "HardwareSerial.h"

#define RXD2 16
#define TXD2 17

const int BUFFER_SIZE = 1;
char buf[BUFFER_SIZE];
String T;
void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  //Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  
}

void loop() {
  // check if data is available
  if (Serial.available() > 0) {
    // read the incoming bytes:
    //T = Serial.read();
    //Serial.readBytes(buf,1); 
    char rlen = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);

    // prints the received data
    Serial.println(" ");
    Serial.print("I received: ");
    Serial.print(buf[0]);
    //Serial.print(buf[0]);
//    for(int i = 0; i < rlen; i++)
//      Serial.print(buf[i]);
  }
}
