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

int Num; //variable de .TXT que quieren ver
int control; //para que se ejecute hast que el usuario decida terminar

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
  pinMode(PA_3, OUTPUT);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial.println();
  Serial.println("--------- ARCHIVOS EN LA SD ---------");
  Serial.println();
  myFile = SD.open("/"); //abrir archivos
  printDirectory(myFile, 0); //funcion que muestra archivos dentro de SD
  Serial.println("");
  Serial.println("-----------------------------------------------------");
  Serial.println("¿Qué archivo .TXT  quieres ver? Escribir 1, 2, 3 o 4");
  Serial.println("-----------------------------------------------------");
  
}

void loop(){
  
  while (control != 5){//mientras no se escriba 5, el programa corre 
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
        Serial.println("");
        Serial.println("-------------------------------------------------------------");//volver a preguntar
        Serial.println("Escribir 1, 2, 3 o 4 para ver otro archivo o 5 para terminar");
        Serial.println("-------------------------------------------------------------");
      } 
       
      else if (Num == '2'){ //si es opcion 2, mostrar archivo 2 (corazon)
          myFile = SD.open("corazon.txt");
        if (myFile) {
          Serial.println();
          Serial.println("------------ Corazón ------------");
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
        Serial.println("");
        Serial.println("-------------------------------------------------------------"); //volver a preguntar
        Serial.println("Escribir 1, 2, 3 o 4 para ver otro archivo o 5 para terminar");
        Serial.println("-------------------------------------------------------------");
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
        Serial.println("");
        Serial.println("-------------------------------------------------------------"); //volver a preguntar
        Serial.println("Escribir 1, 2, 3 o 4 para ver otro archivo o 5 para terminar");
        Serial.println("-------------------------------------------------------------");
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
        //Serial.println();
        Serial.println("");
        Serial.println("-------------------------------------------------------------"); //volver a preguntar
        Serial.println("Escribir 1, 2, 3 o 4 para ver otro archivo o 5 para terminar");
        Serial.println("-------------------------------------------------------------");
      }
   else if (Num == '5'){
      Serial.println("");
      Serial.println("Fin del programa");
      control = 5;
    }
    else{}
    }
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
