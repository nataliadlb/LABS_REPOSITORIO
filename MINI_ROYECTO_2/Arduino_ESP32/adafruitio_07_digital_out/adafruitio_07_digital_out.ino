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

String piloto  = " ";
int cont = 0;
int hola = 0;

AdafruitIO_Feed *LedPiloto1Feed = io.feed("LedPiloto1"); //DIGITAL
AdafruitIO_Feed *LedPiloto2Feed = io.feed("LedPiloto2"); //DIGITAL
AdafruitIO_Feed *ContadorFeed = io.feed("contador");

void setup() {

  //Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600);
  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
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

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  

  // increment the count by 1
  //cont++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

  if(Serial2.available()){
      cont = (int)Serial2.read();
      Serial.print("sending -> ");
      Serial.println(cont);
      ContadorFeed->save(cont);
      //Serial.println(cont);
     //Serial2.println(hola);
     //Serial2.write(Serial.read());  
   }

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
  }
  else{
    Serial.println("OFF");
    piloto = "2";
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

  }
  else{
    Serial.println("OFF");
    piloto = "4";
    
  }
  Serial.println(" ");
  Serial.print("valor: ");
  Serial.println(piloto);
  Serial.println(" ");
}
