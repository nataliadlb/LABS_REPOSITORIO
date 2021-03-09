
String piloto  = " ";
//char cont;
int hola = 0;
#include "HardwareSerial.h"

#define RXD2 16
#define TXD2 17

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // start the serial connection
  Serial.begin(9600);
  // wait for serial monitor to open

}

void loop() {
//  Serial.println(cont);
//  delay(500);
//  cont++;


  while(Serial.available()>0){
      char cont = Serial.read();
      Serial.print("sending -> ");
      Serial.println(cont);
   }

}

  
