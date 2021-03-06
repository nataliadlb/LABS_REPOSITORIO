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
#include "HardwareSerial.h"
/************************ Example Starts Here *******************************/


const int BUFFER_SIZE = 14;
char Time[BUFFER_SIZE];
char Date[BUFFER_SIZE];
int tog = 0;
String send_data;
//uint8_t rec_data = 0;



#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

AdafruitIO_Feed *LedPiloto1Feed = io.feed("LedPiloto1"); //DIGITAL
AdafruitIO_Feed *LedPiloto2Feed = io.feed("LedPiloto2"); //DIGITAL
AdafruitIO_Feed *TimeFeed = io.feed("Time");
AdafruitIO_Feed *DateFeed = io.feed("Date");

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
  DateFeed->save(Date);//Mandar la fecha a adafruit
}

void loop() {

  io.run();
  TimeFeed->save(Time); //Mandar valor de la hora a Adafruit
  
   if(Serial2.available()>0){

      Serial.print("Mandando: "); //Dependiendo de que led en adafruit se enciende o apaga
      if (send_data == "1"){
        Serial2.write((char)49);
        Serial.println((char)49);
        }
      else if (send_data == "2"){
        Serial2.write((char)50);
        Serial.println((char)50);
        }
        else if (send_data == "3"){
        Serial2.write((char)51);
        Serial.println((char)51);
        }
        else if (send_data == "4"){
        Serial2.write((char)52);
        Serial.println((char)52);
        }

        if (tog == 0){ //constante toggle para guardar el valor de la fecha y de la hora
          int rlen_T = Serial.readBytesUntil('\n', Time, BUFFER_SIZE);
          Serial.print("Recibiendo: ");
          for(int i = 0; i < rlen_T; i++){
          Serial.print(Time[i]);
          tog = 1;
          }
        }
         else if (tog == 1){
          int rlen_D = Serial.readBytesUntil('\n', Date, BUFFER_SIZE);
          Serial.print("Recibiendo: ");
          for(int i = 0; i < rlen_D; i++){
            Serial.print(Date[i]);
          }
          tog = 0;
         }   
 
       Serial.println(" ");
   }
 
  delay(500);
}
 
// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.

void handleMessage1(AdafruitIO_Data *data) {
  
  if(data->toString() == "ON"){
    //Serial.println("HIGH");
    send_data = "1";
  }
  else{
    //Serial.println("OFF");
    send_data = "2";
  } 
}

void handleMessage2(AdafruitIO_Data *data) {
  
  if(data->toString() == "ON"){
    //Serial.println("HIGH");
    send_data = "3";

  }
  else{
    //Serial.println("OFF");
    send_data = "4";  
  }
}
