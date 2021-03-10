// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <stdint.h>
#include <stdio.h>

/************************ Example Starts Here *******************************/


const int BUFFER_SIZE = 1;
char buf[BUFFER_SIZE];
uint8_t cont = 0;
String send_data;
uint8_t rec_data = 0;
bool rec = false;

//char[14] Time ;
//char[14] Date;
//char[14] Recibir;

#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

AdafruitIO_Feed *LedPiloto1Feed = io.feed("LedPiloto1"); //DIGITAL
AdafruitIO_Feed *LedPiloto2Feed = io.feed("LedPiloto2"); //DIGITAL
AdafruitIO_Feed *ContadorFeed = io.feed("contador");

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  
  // wait for serial monitor to open
  while(! Serial){
    ;
    }
  
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  
  LedPiloto1Feed->onMessage(handleMessage1);
  LedPiloto2Feed->onMessage(handleMessage2);
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  LedPiloto1Feed->get();
  LedPiloto2Feed->get();
}

void loop() {

  io.run();
  
   if(Serial2.available()>0){
      Serial.print("valor: ");
      if (send_data == "1"){
        Serial.write((char)49);
//        Serial.println(" ");
//        Serial.println((char)49);
//        Serial.println(" ");
        }
      else if (send_data == "2"){
        Serial2.write((char)50);
        Serial.println(" ");
        Serial.println("RA6 OFF");
        Serial.println(" ");
        }
        else if (send_data == "3"){
        Serial2.write((char)51);
        Serial.println(" ");
        Serial.println("RA7 ON");
        Serial.println(" ");
        }
        else if (send_data == "4"){
        Serial2.write((char)52);
        Serial.println(" ");
        Serial.println("RA7 OFF");
        Serial.println(" ");
        }
      //Serial2.write(Serial.read());
      
     
     //Serial2.write(Serial.read());  
   }
 
//if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
//    ContadorFeed->save(cont);
//    // increment the count by 1
//    // after publishing, store the current time
//    lastUpdate = millis();
//
//    if (rec) {
//      //  Serial.println(" - TRANSMITTING");
//      Serial.println(send_data);
//    }
//    else {
//      //  Serial.println("");
//    }
//    rec = false;
//  }
  
//   if(Serial2.available()){
//      cont = (uint8_t)Serial2.read();
//      Serial.print("sending -> ");
//      Serial.println(cont);
//      ContadorFeed->save(cont);
//      Serial2.write(piloto); 
//   }
  delay(3000);
}

  
// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {
//  Serial.println("--------");
//  Serial.println("Piloto 1");
//  Serial.print("received <- ");
  //Serial.println(data->value());
  
  if(data->toString() == "ON"){
    //Serial.println("HIGH");
    send_data = "1";
    //Serial2.write('1'); //49 EN ASCII
  }
  else{
    //Serial.println("OFF");
    send_data = "2";
    //Serial2.write('2');
  }
  
}

void handleMessage2(AdafruitIO_Data *data) {
//  Serial.println("--------");
//  Serial.println("Piloto 2");
//  Serial.print("received <- ");
  //Serial.println(data->value());
  
  if(data->toString() == "ON"){
    //Serial.println("HIGH");
    send_data = "3";

  }
  else{
    //Serial.println("OFF");
    send_data = "4";
    
  }
 
}
