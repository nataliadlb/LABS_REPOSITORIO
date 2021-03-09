
String piloto  = " ";
String cont;
int hola = 0;
#include "HardwareSerial.h"

#define RXD2 16
#define TXD2 17

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // start the serial connection
  Serial.begin(9600);
  // wait for serial monitor to open
  while (!Serial2) {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop() {
//  Serial.println(cont);
//  delay(500);
//  cont++;


  while(Serial2.available()>0){
      cont = Serial2.read();
      Serial.print("sending -> ");
      Serial.println(cont);
   }

}

  
