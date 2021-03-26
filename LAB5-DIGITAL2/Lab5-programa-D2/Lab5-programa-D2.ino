/*
  Labortorio # 5
  Natali de León Berci´n
  carné: 18193
  Digital 2
  sección 20
  marzo 2021
*/

/*
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 2 Feb 2014
 by Scott Fitzgerald
 
 This example code is in the public domain.
 */
 
#include <SPI.h>
#include <SD.h>

int Num;
File myFile;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  SPI.setModule(0);

  Serial.println(" ");
  Serial.println("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PA_3, OUTPUT);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial.println();
  Serial.println("--------- ARCHIVOS EN LA SD ---------");
  Serial.println();
  myFile = SD.open("/");
  printDirectory(myFile, 0);
  Serial.println("");
  Serial.println("-----------------------------------------------------");
  Serial.println("¿Qué archivo .TXT  quieres ver? Escribir 1, 2, 3 o 4");
  Serial.println("-----------------------------------------------------");
  
}

void loop(){
    if (Serial.available() > 0) { //Leer valor que se ingresa
     Num = Serial.read();
    }
    
      if (Num == '1'){ //si es opcion 1, mostrar archivo 1 (pacman)
         myFile = SD.open("pacman.txt");
        if (myFile) {
          Serial.println();
          Serial.println("------------ Pacman ------------");
          Serial.println();
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
        }
      } 
       
      else if (Num == '2'){ //si es opcion 2, mostrar archivo 2 (corazon)
          myFile = SD.open("corazon.txt");
        if (myFile) {
          Serial.println();
          Serial.println("------------ Corazon ------------");
          Serial.println();
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        } else {
          // if the file didn't open, print an error:
          
          Serial.println("error opening test.txt");
        }
      }
        
    else if (Num == '3'){ //si es opcion 3, mostrar archivo 3 (ying yang)
      myFile = SD.open("yingyang.txt");
        if (myFile) {
          Serial.println();
          Serial.println("----------- Ying yang -----------");
          Serial.println();
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
        }
      }

   else if (Num == '4'){ //si es opcion 4, mostrar archivo 4 (fantasma pacman)
      myFile = SD.open("fantasma.txt");
        if (myFile) {
          Serial.println();
          Serial.println("------- Fantasma de pacman -------");
          Serial.println();
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
            
          }
          // close the file:
          myFile.close();
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
        }
        Serial.println();
      }
    else{}
 }
        

void printDirectory(File dir, int numTabs) {//funcion que muestra los archivos de la SD 
   while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
  Serial.println("");

}
