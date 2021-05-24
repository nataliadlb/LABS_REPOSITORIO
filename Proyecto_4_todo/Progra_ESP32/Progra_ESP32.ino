/*************************************************************************************************
/**
 Proyecto # 4
 Natalia de León Bericán
 carne: 18193
 Digital 2
 Sección 20
 Mayo 2021
*/
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
const char* password = "*******";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

int Val = 57; // for incoming serial data
int p1 = 0;
int p2 = 0;
int p3 = 0;
int p4 = 0;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

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
  if (Serial2.available() > 0) {
    // read the incoming byte:
    Val = Serial2.read();
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  //Serial.println("INICIO");
  server.send(200, "text/html", SendInicioHTML(Val));
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendInicioHTML(uint8_t Valor) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Parqueos</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 50px auto; text-align: center;}\n";
  ptr += ".column {float: left ; width: 25%; pading: 10px}\n";
  ptr += "row:after {content:""; display: table; clear: both}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  
  ptr += "<body>\n";
  ptr += "<h1 style=\"font-size:48px\">Control de parqueos  &#128663</h1>\n";
  
  ptr += "<div class=\"row\">\n"; // crear 4 columnas para los titulos de los parqueos
  
    ptr += "<div class=\"column\">\n";
      ptr += "<h2 style=\"font-size:32px\"> PARQUEO 1 </h2>\n";
      if (Valor == 48){ //si se ingresa el valor 0, quiere decir que esta ocupado P1
        p1 = 1;
        } 
      if (Valor == 49){
        p1 = 0;
        } 
      if (p1 == 0){
        p1 = 0;
        ptr += "<p style=\"font-size:25px\">LIBRE</p>\n";
        ptr += "<p style=\"font-size:25px\">&#9989</p>\n";
        }
      if (p1 == 1){
        p1 = 1;
        ptr += "<p style=\"font-size:25px\">OCUPADO</p>\n";
        ptr += "<p style=\"font-size:25px\">&#10060</p>\n";
        }

    ptr += "</div>";

    ptr += "<div class=\"column\">\n";
      ptr += "<h2 style=\"font-size:32px\"> PARQUEO 2 </h2>\n";
      if (Valor == 50 ){
        p2 = 1;      
        } 
      if (Valor == 51){
        p2 = 0;
        }
      if (p2 == 0){
        p2 = 0;
        ptr += "<p style=\"font-size:25px\">LIBRE</p>\n";
        ptr += "<p style=\"font-size:25px\">&#9989</p>\n";
        }
      if (p2 == 1){
        p2 = 1;
        ptr += "<p style=\"font-size:25px\">OCUPADO</p>\n";
        ptr += "<p style=\"font-size:25px\">&#10060</p>\n";
        } 
    ptr += "</div>";

    ptr += "<div class=\"column\">\n";
      ptr += "<h2 style=\"font-size:32px\"> PARQUEO 3 </h2>\n";
      if (Valor == 52){
        p3 = 1;
        } 
      if (Valor == 53){
        p3 = 0;
        }
      if (p3 == 0){
        p3 = 0;
        ptr += "<p style=\"font-size:25px\">LIBRE</p>\n";
        ptr += "<p style=\"font-size:25px\">&#9989</p>\n";
        }
      if (p3 == 1){
        p3 = 1;
        ptr += "<p style=\"font-size:25px\">OCUPADO</p>\n";
        ptr += "<p style=\"font-size:25px\">&#10060</p>\n";
        } 
    ptr += "</div>";

    ptr += "<div class=\"column\">\n";
      ptr += "<h2 style=\"font-size:32px\"> PARQUEO 4 </h2>\n";
      if (Valor == 54){
        p4 = 1;
        } 
      if (Valor == 55){
        p4 = 0;
        }
      if (p4 == 0){
        p4 = 0;
        ptr += "<p style=\"font-size:25px\">LIBRE</p>\n";
        ptr += "<p style=\"font-size:25px\">&#9989</p>\n";
        }
      if (p4 == 1){
        p4 = 1;
        ptr += "<p style=\"font-size:25px\">OCUPADO</p>\n";
        ptr += "<p style=\"font-size:25px\">&#10060</p>\n";
        } 
    ptr += "</div>";
  
  ptr += "</div>";
  // refresh
  ptr += "<script>\n";
  ptr += "<!--\n";
  ptr += "function timedRefresh(timeoutPeriod) {\n";
  ptr += "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "window.onload = timedRefresh(1000);\n";
  ptr += "\n";
  ptr += "//   -->\n";
  ptr += "</script>\n";
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
