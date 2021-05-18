/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
#include <stdint.h>
#include <stdio.h>
#include "HardwareSerial.h"
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "CASA WIFI 2";  // Enter your SSID here
const char* password = "casa2020";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)


uint8_t LED1pin = 2;
bool LED1status = LOW;
int Val = 0; // for incoming serial data

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(LED1pin, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.on("/led1on", handle_led1on);
  //server.on("/led1off", handle_led1off);
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  // reply only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    Val = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(Val, DEC);
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  Serial.println("Mandando información...");
  server.send(200, "text/html", SendHTML(Val));
}
//************************************************************************************************
// Handler de led1on
//************************************************************************************************
void handle_led1on() {
  Serial.println("Mandando información...");
  server.send(200, "text/html", SendHTML(Val));
}
////************************************************************************************************
//// Handler de led1off
////************************************************************************************************
//void handle_led1off() {
//  LED1status = LOW;
//  Serial.println("GPIO2 Status: OFF");
//  server.send(200, "text/html", SendHTML(LED1status));
//}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t Valor) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Parqueos</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 50px auto; text-align: center;}\n";
  ptr += ".column {float: left ; width: 25%; pading: 10px}\n";
  ptr += "row:after {content:""; display: table; clear: both}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  
  ptr += "<body>\n";
  ptr += "<h1>Control de parqueos &#128664</h1>\n";
  
  ptr += "<div class=\"row\">\n"; // crear 4 columnas para los titulos de los parqueos
  
    ptr += "<div class=\"column\">\n";
      ptr += "<h2> PARQUEO 1 </h2>\n";
      if (Valor == 48){
        ptr += "<p>OCUPADO</p>\n";
        } 
      else if (Valor == 49){
        ptr += "<p>LIBRE</p>\n";
        } 
    ptr += "</div>";

    ptr += "<div class=\"column\">\n";
      ptr += "<h2> PARQUEO 2 </h2>\n";
      if (Valor == 50){
        ptr += "<p>OCUPADO</p>\n";
        } 
      else if (Valor == 51){
        ptr += "<p>LIBRE</p>\n";
        } 
    ptr += "</div>";

    ptr += "<div class=\"column\">\n";
      ptr += "<h2> PARQUEO 3 </h2>\n";
      if (Valor == 52){
        ptr += "<p>OCUPADO</p>\n";
        } 
      else if (Valor == 53){
        ptr += "<p>LIBRE</p>\n";
        } 
    ptr += "</div>";

    ptr += "<div class=\"column\">\n";
      ptr += "<h2> PARQUEO 4 </h2>\n";
      if (Valor == 54){
        ptr += "<p>OCUPADO</p>\n";
        } 
      else if (Valor == 55){
        ptr += "<p>LIBRE</p>\n";
        } 
    ptr += "</div>";
  
  ptr += "</div>";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
