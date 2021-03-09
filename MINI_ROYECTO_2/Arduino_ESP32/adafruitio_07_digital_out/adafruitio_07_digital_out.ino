// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
#define RXD2 16
#define TXD2 17
#define IO_LOOP_DELAY 5000

unsigned long lastUpdate = 0;

String piloto  = " ";
String cont;
int hola = 0;

//char[14] Time ;
//char[14] Date;
//char[14] Recibir;

AdafruitIO_Feed *LedPiloto1Feed = io.feed("LedPiloto1"); //DIGITAL
AdafruitIO_Feed *LedPiloto2Feed = io.feed("LedPiloto2"); //DIGITAL
AdafruitIO_Feed *ContadorFeed = io.feed("contador");

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  // wait for serial monitor to open
  while(! Serial);

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
      cont = Serial2.read();
      Serial.print("sending -> ");
      Serial.println(cont);
      ContadorFeed->save(cont);
   }
//   
//  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
//    // save count to the 'counter' feed on Adafruit IO
//    Serial.print("sending -> ");
//    Serial.println(cont);
//    ContadorFeed->save(cont);
//  }
// 
//
//    // after publishing, store the current time
//    lastUpdate = millis();
    
  delay(3000);
}

  
// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {
  Serial.println("--------");
  Serial.println("Piloto 1");
  Serial.print("received <- ");
  //Serial.println(data->value());
  
  if(data->toString() == "ON"){
    Serial.println("HIGH");
    piloto = "1";
    Serial2.write('1'); //49 EN ASCII
  }
  else{
    Serial.println("OFF");
    piloto = "2";
    Serial2.write('2');
  }
  Serial.println(" ");
  Serial.print("valor: ");
  Serial.println(piloto);
  Serial.println(" ");
}

void handleMessage2(AdafruitIO_Data *data) {
  Serial.println("--------");
  Serial.println("Piloto 2");
  Serial.print("received <- ");
  //Serial.println(data->value());
  
  if(data->toString() == "ON"){
    Serial.println("HIGH");
    piloto = "3";
    Serial2.write('3');

  }
  else{
    Serial.println("OFF");
    piloto = "4";
    Serial2.write('4');
    
  }
  Serial.println(" ");
  Serial.print("valor: ");
  Serial.println(piloto);
  Serial.println(" ");
}
