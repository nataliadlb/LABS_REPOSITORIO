// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
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

String piloto  = "";
//String piloto2  = "';
int cont = 0;

// set up the 'digital' feed
//AdafruitIO_Feed *digital = io.feed("digital");
AdafruitIO_Feed *LedPiloto1Feed = io.feed("LedPiloto1"); //DIGITAL
AdafruitIO_Feed *LedPiloto2Feed = io.feed("LedPiloto2"); //DIGITAL
AdafruitIO_Feed *ContadorFeed = io.feed("contador");

void setup() {
  
  // start the serial connection
  Serial.begin(115200);

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
  //ContadorFeed->onMessage(sendValue);
  
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
  Serial.print("sending -> ");
  Serial.println(cont);
  ContadorFeed->save(cont);



  // increment the count by 1
  cont++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
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
    piloto = "P11";
  }
  else{
    Serial.println("OFF");
    piloto = "P10";
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
    piloto = "P21";

  }
  else{
    Serial.println("OFF");
    piloto = "P20";
    
  }
  Serial.println(" ");
  Serial.print("valor: ");
  Serial.println(piloto);
  Serial.println(" ");
}
