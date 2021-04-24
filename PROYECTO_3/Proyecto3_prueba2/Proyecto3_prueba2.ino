#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include <SPI.h>
#include <SD.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"


#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"
#include "pitches.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1

#define PUSH_LEFT_J1 PA_7
#define PUSH_RIGTH_J1 PE_3
#define PUSH_UP_J1 PA_6
#define PUSH_DOWN_J1 PE_2

#define PUSH_LEFT_J2 PD_7
#define PUSH_RIGTH_J2  PC_7
#define PUSH_UP_J2  PD_6
#define PUSH_DOWN_J2 PC_6
 
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7}; 

File myFile;


//--- BANDERAS ---//
volatile byte flag_jugar = LOW; //BANDERA QUE INDICA QUE YA TERMINARON DE ESCOGER
volatile byte flag_boton_jugar = LOW;
volatile byte ganar_N1 = LOW;
volatile byte ganar_N2 = LOW;
volatile byte ganar_N3 = LOW;
volatile byte Listo_per_J1 = LOW;
volatile byte Listo_per_J2 = LOW;
volatile byte JUEGO_EN_PROGRESO = LOW;

//--- BANDERAS PUSH MOVIMIENTO ---//
volatile byte b_LEFT_J1 = LOW;
volatile byte b_LEFT_J2 = LOW;
volatile byte b_RIGHT_J1 = LOW;
volatile byte b_RIGHT_J2 = LOW;
volatile byte b_UP_J1 = LOW;
volatile byte b_UP_J2 = LOW;
volatile byte b_DOWN_J1 = LOW;
volatile byte b_DOWN_J2 = LOW;

//--- CONTADORES ---//
int cont_PUSH1 = 0;
int cont_personajes_J1 = 0;
int cont_personajes_J2 = 0;

//--- VARIABLES DE INFO ---//
int nivel = 0;
int num_personaje_J1 = 0;
int num_personaje_J2 = 0;
int posicion_J1 = 0;
int posicion_J2 = 0;

//--- PUSH TIVA ---//
const byte interruptPin1 = PUSH1; 
const byte interruptPin2 = PUSH2;
const byte led_VERDE = GREEN_LED; 

//--- ARREGLOS EN SD ---//
unsigned char personajes_inicio_50[10000]={0};

unsigned char J1_Abajo_Derecha[2304]={0};
unsigned char J2_Abajo_Derecha[2304]={0};
unsigned char J1_Arriba_Derecha[1152]={0};
unsigned char J2_Arriba_Derecha[1152]={0};
unsigned char J1_Izquierda_Arriba[1152]={0};
unsigned char J2_Izquierda_Arriba[1152]={0};
unsigned char J1_Izquierda_Abajo[1152]={0};
unsigned char J2_Izquierda_Abajo[1152]={0};

//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
//--- FUNCIONES LCD ---//
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

//---- FUNCIONES PANTALLA INICIO ----//
void Static_Pantalla_Inicio(void); //lo que aparece en el menu y es fijo
void Mov_Pantalla_inicio(void); // lo que se mueve en la pagina de inicio
void Listo_personajes(void); //Mostrar texto de listo cuando se elige personaje

//---- FUNCIONES PANTALLA DE NIVEL ----//
void Nivel_pantalla(int Num_Nivel); //para mostrar la pantalla del nivel que toca

//---- FUNCIONES POSICIONES INICIALES DE OBJETOS EN MAPAS ----//
void Posicion_inicial_munecos(int nivel_pos_i); //funcion para poner o munecos 
void Posicion_meta(int nivel_pos_i); //funcion para poner la meta en cada mapa
void Posicion_estrellas(int nivel_pos_i); //funcion par aponer las estrellas en cada nivel

//---- FUNCION DE CADA MAPA (BLOQUES, ESTRELLAS, ENEMIGOS) ----//
void Mapa_nivel(int nivel_mapa);

//---- FUNCIONES PARA ESCOGER PERSONAJE EN TODO EL JEUGO ----//
void Personajes_usar(int num_per_J1, int num_per_J2);

//--- FUNCIONES PARA SD ---//
void open_SD_bitmap(unsigned char Bitmap_SD[], unsigned long Size_bitmap, char* filename);
int ACII_to_HEX(char *puntero);

//--- FUNCIONES AL APACHAR CADA BOTON EN CADA JUGADOR ---//
void switch_posicion_LEFT_J1(int num_nivel);
void switch_posicion_RIGHT_J1(int num_nivel);
void switch_posicion_UP_J1(int num_nivel);
void switch_posicion_DOWN_J1(int num_nivel);

void switch_posicion_LEFT_J2(int num_nivel);
void switch_posicion_RIGHT_J2(int num_nivel);
void switch_posicion_UP_J2(int num_nivel);
void switch_posicion_DOWN_J2(int num_nivel);


//--- GRAFICOS ---//
//extern uint8_t fondo[];
extern uint8_t dec_nivel_32[];
//extern uint8_t circulo_mov[];
extern uint8_t next_amarillo_24[];
extern uint8_t Bloque_8_celeste[];
extern uint8_t Bloque_8_morado[];
extern uint8_t estrella_16[];

extern uint8_t Muneco_50[];
extern uint8_t Koala_50[];
extern uint8_t Calavera_50[];
extern uint8_t Mono_50[];

extern uint8_t Muneco_Arriba_Derecha[];
extern uint8_t Koala_Arriba_Derecha[];
extern uint8_t Calavera_Arriba_Derecha[];
extern uint8_t Mono_Arriba_Derecha[];

extern uint8_t Muneco_Izquierda_Arriba[];
extern uint8_t Koala_Izquierda_Arriba[];
extern uint8_t Calavera_Izquierda_Arriba[];
extern uint8_t Mono_Izquierda_Arriba[];

extern uint8_t Muneco_Izquierda_Abajo[];
extern uint8_t Koala_Izquierda_Abajo[];
extern uint8_t Calavera_Izquierda_Abajo[];
extern uint8_t Mono_Izquierda_Abajo[];

void setup() {
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(PUSH_LEFT_J1, INPUT_PULLUP);
  pinMode(PUSH_LEFT_J2, INPUT_PULLUP);
  pinMode(PUSH_RIGTH_J1, INPUT_PULLUP);
  pinMode(PUSH_RIGTH_J2, INPUT_PULLUP);
  pinMode(PUSH_UP_J1, INPUT_PULLUP);
  pinMode(PUSH_UP_J2, INPUT_PULLUP);
  pinMode(PUSH_DOWN_J1, INPUT_PULLUP);
  pinMode(PUSH_DOWN_J2, INPUT_PULLUP);

   SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  SPI.setModule(0);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");

  //--- INICIALIZACION SD ---//
  pinMode(PA_3, OUTPUT);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("/"); //abrir archivos
  //printDirectory(myFile, 0); //funcion que muestra archivos dentro de SD
  Serial.println("done!");

  //--- INTERRUPCIONES ---//
  attachInterrupt(digitalPinToInterrupt(interruptPin1), IRS_PUSH1, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), IRS_PUSH2, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_LEFT_J1), LEFT_J1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_LEFT_J2), LEFT_J2, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_RIGTH_J1), RIGTH_J1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_RIGTH_J2), RIGTH_J2, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_UP_J1), UP_J1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_UP_J2), UP_J2, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_DOWN_J1), DOWN_J1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUSH_DOWN_J2), DOWN_J2, FALLING);

  LCD_Init();
  LCD_Clear(0x0000);
  //--- Pantalla de inicio ---//
  Static_Pantalla_Inicio();
}

void loop() {

  if (flag_boton_jugar == LOW && JUEGO_EN_PROGRESO == LOW){
    Mov_Pantalla_inicio();
    Listo_personajes();
    }
  else if (flag_boton_jugar == HIGH && JUEGO_EN_PROGRESO == LOW) {
    String text1 = "JUGAR";
    
    LCD_Print(text1, 111, 200, 2, 0x000, 0x07FF);

    Personajes_usar(num_personaje_J1, num_personaje_J2);
    //flag_boton_jugar = LOW;
    JUEGO_EN_PROGRESO = HIGH;
    delay(500);
    }
 //else {}

  while (JUEGO_EN_PROGRESO == HIGH){
    switch (nivel){
      case 1:
        Nivel_pantalla(1);
        delay(500);
        Mapa_nivel(1);
        while (ganar_N1 != HIGH){
        
        //-- MOVIMIENTOS JUGADOR 1 --//  
        if (b_LEFT_J1 == HIGH){
          switch_posicion_LEFT_J1(1);
          }

//        if (b_RIGTH_J1 == HIGH){
//          switch_posicion_RIGHT_J1(1);
//          }
//
//        if (b_UP_J1 == HIGH){
//          switch_posicion_UP_J1(1);
//          } 
//          
//       if (b_DOWN_J1 == HIGH){
//          switch_posicion_DOWN_J1(1);
//          } 
//
//          
        //-- MOVIMIENTOS JUGADOR 2 --//  
       if (b_LEFT_J2 == HIGH){
          switch_posicion_LEFT_J2(1);
          }
//
//        if (b_RIGTH_J2 == HIGH){
//          switch_posicion_RIGHT_J2(1);
//          }
//
//        if (b_UP_J2 == HIGH){
//          switch_posicion_UP_J2(1);
//          } 
//          
//       if (b_DOWN_J2 == HIGH){
//          switch_posicion_DOWN_J2(1);
//          } 
          }
        //nivel = 2;
      break;
      case 2:
        Nivel_pantalla(2);
        delay(500);
        Mapa_nivel(2);
        delay(1000);
        //nivel = 3;
      break;
      case 3:
        Nivel_pantalla(3);
        delay(500);
        Mapa_nivel(3);
        delay(1000);
        //nivel = 4;
      break;

      case 4:
        Mapa_nivel(4);
        delay(1000);
        //nivel = 1;
      break;
      }
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//                                                        FUNCIONES PROPIAS                                                                  //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//-----------------------------------------------              INTERRUPCIONES              -----------------------------------------------//
 
//***************************************************************************************************************************************
// Función interrupcion para comenzar a jugar
//***************************************************************************************************************************************
void IRS_PUSH1() { //INTERRUPCION PUSH1: CAMBIAR PERSONAJES Y COMENZAR A JUGAR
  Listo_per_J1 = HIGH;
  cont_PUSH1++; //LUEGO DE PRESIONAR UNA VEZ PARA ESCOGER EL PERSONAJE SI SE VUELVE A PRESIONAR YA ES PARA JUGAR
  
  if(Listo_per_J1 == HIGH & Listo_per_J2 == HIGH & cont_PUSH1 !=1){ 
    flag_boton_jugar = HIGH;
    nivel = 1;
    cont_PUSH1 = 0;
    
    }
}

void IRS_PUSH2() { //INTERRUPCION PUSH2
   Listo_per_J2 = HIGH;
}

void LEFT_J1() { //AQUI TAMBIEN CAMBIA PARA ESCOGER PERSONAJE J1
  if (Listo_per_J1 == LOW && JUEGO_EN_PROGRESO == LOW){
      cont_personajes_J1++;
      Rect(28,110,25,25,0x07FF);
    if (cont_personajes_J1 == 4){
      cont_personajes_J1 = 0;
    }
  }
  if (JUEGO_EN_PROGRESO == HIGH){
    b_LEFT_J1 = HIGH;
  }
}

void LEFT_J2() { 
  b_LEFT_J2 = HIGH;
  //ganar_N1 = HIGH;
  //nivel = 2; 
}

void RIGTH_J1() { 
  //ganar_N2 = HIGH;
  //nivel = 3;
}

void RIGTH_J2() { //AQUI TAMBIEN CAMBIA PARA ESCOGER PERSONAJE J2
  if (Listo_per_J2 != HIGH && JUEGO_EN_PROGRESO == LOW){
      cont_personajes_J2++;
      Rect(258,110,25,25,0x07FF);
    if (cont_personajes_J2 == 4){
      cont_personajes_J2 = 0;
    }
  }
    else if (Listo_per_J2 == HIGH && JUEGO_EN_PROGRESO == HIGH){
    b_RIGHT_J2 = HIGH;
  }
}

void UP_J1() { 
  //digitalWrite(led_VERDE, HIGH);
}

void UP_J2() { 
  //digitalWrite(led_VERDE, HIGH);
}

void DOWN_J1() { 
  //digitalWrite(led_VERDE, HIGH);
}

void DOWN_J2() { 
  //digitalWrite(led_VERDE, HIGH);
}
//***************************************************************************************************************************************
// Función para el menú de inicio
//***************************************************************************************************************************************
void Static_Pantalla_Inicio(void){
  
    //FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
    FillRect(0, 0, 319, 239, 0x0000);         //FONDO
    String text_escoge_J = "ELIGE PERSONAJE"; //TEXTOS DE LA PANTALLA
    String text_boton_jugar = "JUGAR";
    String text_J1 = "J1";
    String text_J2 = "J2";
    // LCD_Print(String text, int x, int y, int fontSize, int color, int background);
    LCD_Print(text_escoge_J, 45, 20, 2,0x0000, 0xFF40); //MOSTRAR LOS TEXTOS
    LCD_Print(text_J1, 75, 60, 2, 0xFF40, 0x0000);
    LCD_Print(text_J2, 205, 60, 2, 0xFF40, 0x0000);
    LCD_Print(text_boton_jugar, 111, 200, 2, 0x000, 0xFF40); //TEXTO DE BOTON JUGAR
    //void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
    Rect(63,95,56,56,0xFF40); // MARCOS DE LOS PERSONAJES
    Rect(193,95,56,56,0xFF40);
    for(int y = 0; y <239; y++){ //DECORACION DE LOS LADOS
          LCD_Bitmap(0, y, 8, 8, Bloque_8_morado); // en medio +-4 a partir del 156
          LCD_Bitmap(312, y, 8, 8, Bloque_8_morado);
          y += 7;
          }   
}

void Mov_Pantalla_inicio(void){ //Todo lo que va cambiando de la pantalla de inicio

    LCD_Sprite(29, 111,24,24,next_amarillo_24,1,0,1,0); //FLECHAS PARA CAMBIAR DE PERSONAJE
    LCD_Bitmap(259, 111, 24, 24, next_amarillo_24);
    switch(cont_personajes_J1){

      case 0:                                                       //EN BASE A LA CANTIDAD DE VECES QUE PRESIONA EL BOTON SE MUESTRA EL PERSONAJE
        //open_SD_bitmap(personajes_inicio_50, 10001, "Mun_50.txt");//POR DEFAULT PERONAJE QUE SE OBTIENE DE LA SD
        LCD_Bitmap(66, 98, 50, 50, Muneco_50);                      //SE MUESTRA EL PERSONAJE
        Rect(28,110,25,25,0x0000);
        num_personaje_J1 = 0;                                       //VARIABLE QUE INDICA CUAL PERSONAJE SE ESCOGIO
        break;
      case 1:
        //open_SD_bitmap(personajes_inicio_50, 10001, "Cal_50.txt");
        LCD_Bitmap(66, 98, 50, 50, Calavera_50);
        Rect(28,110,25,25,0x0000);
        num_personaje_J1 = 1;
        break;
      case 2:
        //open_SD_bitmap(personajes_inicio_50, 10001, "Koala_50.txt");
        LCD_Bitmap(66, 98, 50, 50, Koala_50);
        Rect(28,110,25,25,0x0000);
        num_personaje_J1 = 2;
        break;
      case 3:
        //open_SD_bitmap(personajes_inicio_50, 10001, "Mono_50.txt");
        LCD_Bitmap(66, 98, 50, 50, Mono_50);
        Rect(28,110,25,25,0x0000);
        num_personaje_J1 = 3;
        break;
      }

    // PERSONAJE 2 
    switch(cont_personajes_J2){
      case 0:
        //open_SD_bitmap(personajes_inicio_50, 10001, "Mun_50.txt");
        LCD_Bitmap(196, 98, 50, 50, Muneco_50);
        Rect(258,110,25,25,0x0000);
        num_personaje_J2 = 0;
        break;
      case 1:
        //open_SD_bitmap(personajes_inicio_50, 10001, "Cal_50.txt");
        LCD_Bitmap(196, 98, 50, 50, Calavera_50);
        Rect(258,110,25,25,0x0000);
        num_personaje_J2 = 1;
        break;
      case 2:
        //open_SD_bitmap(personajes_inicio_50, 10001, "Koala_50.txt");
        LCD_Bitmap(196, 98, 50, 50, Koala_50);
        Rect(258,110,25,25,0x0000);
        num_personaje_J2 = 2;
        break;
      case 3:
        //open_SD_bitmap(personajes_inicio_50, 10001, "Mono_50.txt");
        LCD_Bitmap(196, 98, 50, 50, Mono_50);
        Rect(258,110,25,25,0x0000);
        num_personaje_J2 = 3;
        break;
      }
}

//***************************************************************************************************************************************
// Función para seleccionar el bitmap del personaje a usar 
//***************************************************************************************************************************************
void Personajes_usar(int num_per_J1, int num_per_J2){ //EN BASE A LO QUE ESCOGIERON EN EL INICIO SE OBTIENE Y GUARDA DE SD EL PERSONAJE A USAR EN EL MAPA
    //J1//
    switch (num_per_J1){
      
      case 0:
        open_SD_bitmap(J1_Abajo_Derecha, 2305, "Mun_24.txt");
        //J1_Arriba_Derecha = Muneco_Arriba_Derecha;
       //open_SD_bitmap(J1_Arriba_Derecha, 2305, "Mu_UP_D.txt");
//        open_SD_bitmap(J1_Izquierda_Arriba, 2305, "Mu_I_UP.txt");
//        open_SD_bitmap(J1_Izquierda_Abajo, 2305, "Mu_I_DO.txt");
        break;
      case 1:
        open_SD_bitmap(J1_Abajo_Derecha, 2305, "Cal_24.txt");
//        open_SD_bitmap(J1_Arriba_Derecha, 2305, "C_UP_D.txt");
//        open_SD_bitmap(J1_Izquierda_Arriba, 2305, "C_I_UP.txt");
//        open_SD_bitmap(J1_Izquierda_Abajo, 2305, "C_I_DO.txt");
        break;
        
      case 2:
        open_SD_bitmap(J1_Abajo_Derecha, 2305, "Koala_24.txt");
//        open_SD_bitmap(J1_Arriba_Derecha, 2305, "K_UP_D.txt");
//        open_SD_bitmap(J1_Izquierda_Arriba, 2305, "K_I_UP.txt");
//        open_SD_bitmap(J1_Izquierda_Abajo, 2305, "K_I_DO.txt");
        break;

     case 3:
        open_SD_bitmap(J1_Abajo_Derecha, 2305, "Mono_24.txt");
//        open_SD_bitmap(J1_Arriba_Derecha, 2305, "Mo_UP_D.txt");
//        open_SD_bitmap(J1_Izquierda_Arriba, 2305, "Mo_I_UP.txt");
//        open_SD_bitmap(J1_Izquierda_Abajo, 2305, "Mo_I_DO.txt");
        break;
      }

  //J2//
   switch (num_per_J2){
      case 0:
        open_SD_bitmap(J2_Abajo_Derecha, 2305, "Mun_24.txt");
        break;
      case 1:
        open_SD_bitmap(J2_Abajo_Derecha, 2305, "Cal_24.txt");
        break;
        
      case 2:
        open_SD_bitmap(J2_Abajo_Derecha, 2305, "Koala_24.txt");
        break;

     case 3:
        open_SD_bitmap(J2_Abajo_Derecha, 2305, "Mono_24.txt");
        break;
      }
  }
  
//***************************************************************************************************************************************
// Función escribir que estan listos cuando escogen su personaje
//***************************************************************************************************************************************
void Listo_personajes(void){ //MUESTRAN LA PALABRA LISTO DEBAJO DEL PERSONAJE PARA INDICAR QUE YA PUEDEN JUGAR
  if (Listo_per_J1 ==HIGH){
    String text_listo_J1 = "LISTO";
    LCD_Print(text_listo_J1, 72, 160, 1, 0x000, 0x07FF);
    }

  if (Listo_per_J2 ==HIGH){
    String text_listo_J2 = "LISTO";
    LCD_Print(text_listo_J2, 202, 160, 1, 0x000, 0x07FF);
    }
  }

//***************************************************************************************************************************************
// Función para pantalla con titulo del nivel que toca
//***************************************************************************************************************************************
void Nivel_pantalla(int Num_Nivel){ //MOSTRAR EL NIVEL
    FillRect(0, 0, 320, 240, 0x0000);
    String Str_nivel = String(Num_Nivel); //convertir valor INT de cantidad estrellas a STRING
    String text_nivel = "NIVEL " + Str_nivel;
    LCD_Print(text_nivel, 100, 110, 2, 0x07FF, 0x0000);
    
    switch (Num_Nivel){ //DECORACION SEGUN EL NIVEL
      case 1: 
         LCD_Bitmap(144, 160, 32, 32, dec_nivel_32);
         break;

      case 2:
         LCD_Bitmap(118, 160, 32, 32, dec_nivel_32);
         LCD_Bitmap(170, 160, 32, 32, dec_nivel_32);
         break;

      case 3:
         LCD_Bitmap(102, 160, 32, 32, dec_nivel_32);
         LCD_Bitmap(144, 160, 32, 32, dec_nivel_32);
         LCD_Bitmap(186, 160, 32, 32, dec_nivel_32);
         break;
        
      }
    delay(300);   
}

//***************************************************************************************************************************************
// Función hacer colocar los muñecos en su posicion incial en cada mapa
//***************************************************************************************************************************************
void Posicion_inicial_munecos(int nivel_pos_i){ //SEGUN EL NUMERO DE MAPA SE COLOCAN LOS MUNECOS AL INICIO
  
  switch (nivel_pos_i){
    case 1:
      LCD_Sprite(128, 216, 24, 24, J1_Abajo_Derecha,1,0,1,0);
      LCD_Sprite(128+160, 216, 24, 24, J2_Abajo_Derecha,1,0,1,0);
      break;
    case 2:
      LCD_Bitmap(8, 216, 24, 24, J1_Abajo_Derecha); 
      LCD_Bitmap(168, 216, 24, 24, J2_Abajo_Derecha); 
      break; 
    case 3:
      LCD_Sprite(128, 216, 24, 24, J1_Abajo_Derecha,1,0,1,0);
      LCD_Sprite(128+160, 216, 24, 24, J2_Abajo_Derecha,1,0,1,0); 
      break;
  }
}

//***************************************************************************************************************************************
// Función hacer colocar la meta en cada mapa
//***************************************************************************************************************************************
void Posicion_meta(int nivel_pos_i){ //SEGUN EL NUMERO DE MAPA SE COLOCAN LA META
  
  switch (nivel_pos_i){
    case 1:
      //RECTANGULOS DE LUGAR DE SALIDA
      Rect(8,0,24,24,0xFF40); //J1
      Rect(12,4,16,16,0xFF40);
      Rect(16,8,8,8,0xFF40);
      Rect(168,0,24,24,0xFF40);//J2
      Rect(172,4,16,16,0xFF40);
      Rect(176,8,8,8,0xFF40);
      break;
    case 2:
      //RECTANGULOS DE LUGAR DE SALIDA
      Rect(288,0,23,24,0xFF40); //J2
      Rect(292,4,16,16,0xFF40);
      Rect(296,8,8,8,0xFF40);
      
      Rect(128,0,23,24,0xFF40);//J1
      Rect(132,4,16,16,0xFF40);
      Rect(136,8,8,8,0xFF40);
      break; 
    case 3:
      //RECTANGULOS DE LUGAR DE SALIDA
      Rect(72,0,23,24,0xFF40); //J1
      Rect(76,4,16,16,0xFF40);
      Rect(80,8,8,8,0xFF40);
      Rect(232,0,23,24,0xFF40);//J2
      Rect(236,4,16,16,0xFF40);
      Rect(240,8,8,8,0xFF40); 
      break;
  }
}

//***************************************************************************************************************************************
// Función hacer colocar las estrellas en cada mapa
//***************************************************************************************************************************************
void Posicion_estrellas(int nivel_pos_i){ //SEGUN EL NUMERO DE MAPA SE COLOCAN LA META
  
  switch (nivel_pos_i){
    case 1:
      LCD_Bitmap(12, 220, 16, 16, estrella_16);
      LCD_Bitmap(172, 220, 16, 16, estrella_16);
      
      LCD_Bitmap(44, 164, 16, 16, estrella_16);
      LCD_Bitmap(204, 164, 16, 16, estrella_16);

      LCD_Bitmap(132, 148, 16, 16, estrella_16);
      LCD_Bitmap(292, 148, 16, 16, estrella_16);

      LCD_Bitmap(44, 92, 16, 16, estrella_16);
      LCD_Bitmap(204, 92, 16, 16, estrella_16);

      LCD_Bitmap(132, 4, 16, 16, estrella_16);
      LCD_Bitmap(292, 4, 16, 16, estrella_16);
      
      break;
    case 2:
      LCD_Bitmap(92, 220, 16, 16, estrella_16);
      LCD_Bitmap(92+160, 220, 16, 16, estrella_16);
      
      LCD_Bitmap(52, 148, 16, 16, estrella_16);
      LCD_Bitmap(52+160, 148, 16, 16, estrella_16);

      LCD_Bitmap(92, 76, 16, 16, estrella_16);
      LCD_Bitmap(92+160, 76, 16, 16, estrella_16);

      LCD_Bitmap(132, 124, 16, 16, estrella_16);
      LCD_Bitmap(132+160, 124, 16, 16, estrella_16);

      LCD_Bitmap(52, 28, 16, 16, estrella_16);
      LCD_Bitmap(52+160, 28, 16, 16, estrella_16);
      
      break; 
    case 3:
      LCD_Bitmap(12, 196, 16, 16, estrella_16);
      LCD_Bitmap(12+160, 220, 16, 16, estrella_16);
      
      LCD_Bitmap(76, 188, 16, 16, estrella_16);
      LCD_Bitmap(76+160, 188, 16, 16, estrella_16);

      LCD_Bitmap(12, 84, 16, 16, estrella_16);
      LCD_Bitmap(112+160, 84, 16, 16, estrella_16);

      LCD_Bitmap(44, 4, 16, 16, estrella_16);
      LCD_Bitmap(44+160, 4, 16, 16, estrella_16);

      LCD_Bitmap(132, 4, 16, 16, estrella_16);
      LCD_Bitmap(132+160, 4, 16, 16, estrella_16);
      break;
  }
}

//***************************************************************************************************************************************
// Función para los bloques de cada mapa de los niveles
//***************************************************************************************************************************************
void Mapa_nivel(int nivel_mapa){
  FillRect(0, 0, 320, 240, 0x0000);
  switch (nivel_mapa){

// ------------------------------------------------------------//
// ----------------------- MAPA 1 -----------------------------//
// -----------------------------------------------------------//
    case 1:
      Posicion_inicial_munecos(1); //poner a los munecos en su posicion inicial
      Posicion_meta(1);
      Posicion_estrellas(1);

      for(int y = 0; y <240; y++){ // 1 LINEAS DE LAS ORILLAS
          LCD_Bitmap(312, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(160, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(152, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(0, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      for(int x = 240; x <312; x++){ //2
          LCD_Bitmap(x, 208, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x-160, 208, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      for(int x = 200; x <240; x++){ // 3
          LCD_Bitmap(x, 184, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x-160, 184, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      for(int y = 184; y <208; y++){ // 4
          LCD_Bitmap(80, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(240, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }

      for(int y = 216; y <240; y++){ // 5
          LCD_Bitmap(208, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(192, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(48, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(32, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      LCD_Bitmap(40, 216, 8, 8, Bloque_8_morado); //6
      LCD_Bitmap(200, 216, 8, 8, Bloque_8_morado); 
          
      for(int y = 152; y <192; y++){ //7
          LCD_Bitmap(32, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(192, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }

      for(int x = 200; x <280; x++){ //8
          LCD_Bitmap(x, 152, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x-160, 152, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      for(int y = 80; y <128; y++){ //9
          LCD_Bitmap(32, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(192, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }

      for(int y = 48; y <88; y++){ //10
          LCD_Bitmap(56, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(88, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(216, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(248, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      for(int y = 0; y <24; y++){ //11
          LCD_Bitmap(34, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(72, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(194, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(232, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      for(int y = 24; y <56; y++){ //12
          LCD_Bitmap(104, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(120, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(264, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(280, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }

      for(int x = 8; x <32; x++){ //13
          LCD_Bitmap(x, 48, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 48, 8, 8, Bloque_8_morado);
          x += 7;
          }

      for(int x = 40; x <56; x++){ //14
          LCD_Bitmap(x, 80, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 80, 8, 8, Bloque_8_morado);
          x += 7;
          }

      for(int x = 40; x <72; x++){ //15
          LCD_Bitmap(x, 16, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 16, 8, 8, Bloque_8_morado);
          x += 7;
          }

      for(int x = 128; x <152; x++){ //16
          LCD_Bitmap(x, 80, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 80, 8, 8, Bloque_8_morado);
          x += 7;
          }

      for(int y = 40; y <56; y++){ //17
          LCD_Bitmap(112, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(272, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      LCD_Bitmap(96, 48, 8, 8, Bloque_8_morado);
      LCD_Bitmap(256, 48, 8, 8, Bloque_8_morado); 
      LCD_Bitmap(112, 24, 8, 8, Bloque_8_morado); 
      LCD_Bitmap(272, 24, 8, 8, Bloque_8_morado);     
        //delay(1000);
      break;
// ------------------------------------------------------------//
// ----------------------- MAPA 2 -----------------------------//
// -----------------------------------------------------------//
    case 2:
      Posicion_inicial_munecos(2); //poner a los munecos en su posicion inicial
      Posicion_meta(2);
      Posicion_estrellas(2);
      
      for(int y = 0; y <240; y++){ // 1 LINEAS DE LAS ORILLAS
          LCD_Bitmap(312, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(160, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(152, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(0, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      for(int y = 216; y <240; y++){ // 2
          LCD_Bitmap(72, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(80, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(232, y, 8, 8, Bloque_8_morado); 
          LCD_Bitmap(240, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      for(int y = 120; y <168; y++){ // 3
          LCD_Bitmap(72, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(232, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }   
          
      for(int y = 120; y <144; y++){ // 4
          LCD_Bitmap(80, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(240, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }

      for(int y = 144; y <168; y++){ // 5
          LCD_Bitmap(80, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(240, y, 8, 8, Bloque_8_celeste); 
          y += 7;
          }
          
      LCD_Bitmap(32, 176, 8, 8, Bloque_8_morado); //6
      LCD_Bitmap(32+160, 176, 8, 8, Bloque_8_morado);
      
      LCD_Bitmap(120, 176, 8, 8, Bloque_8_morado);
      LCD_Bitmap(120+160, 176, 8, 8, Bloque_8_morado);
      
      LCD_Bitmap(24, 104, 8, 8, Bloque_8_morado);
      LCD_Bitmap(24+160, 104, 8, 8, Bloque_8_morado);
      
      LCD_Bitmap(128, 104, 8, 8, Bloque_8_morado);
      LCD_Bitmap(128+160, 104, 8, 8, Bloque_8_morado);
      
      LCD_Bitmap(32, 56, 8, 8, Bloque_8_morado); //6
      LCD_Bitmap(32+160, 56, 8, 8, Bloque_8_morado);
      
      LCD_Bitmap(120, 56, 8, 8, Bloque_8_morado);
      LCD_Bitmap(120+160, 56, 8, 8, Bloque_8_morado);
        
      for(int x = 32; x <128; x++){ //7
          LCD_Bitmap(x, 184, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(x+160, 184, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(x, 168, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 168, 8, 8, Bloque_8_morado);
          
          LCD_Bitmap(x, 64, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 64, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x, 48, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 48, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      for(int x = 8; x <32; x++){ //8
          LCD_Bitmap(x, 96, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 96, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x, 112, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 112, 8, 8, Bloque_8_morado);
          
          LCD_Bitmap(x+120, 96, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+280, 96, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+120, 112, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+280, 112, 8, 8, Bloque_8_morado);

          LCD_Bitmap(x, 0, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 0, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x, 16, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 16, 8, 8, Bloque_8_morado);
          x += 7;
          }

      for(int y = 72; y <96; y++){ // 9
          LCD_Bitmap(72, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(232, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(80, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(240, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }
          
      for(int y = 24; y <48; y++){ // 10
          LCD_Bitmap(72, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(232, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(80, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(240, y, 8, 8, Bloque_8_celeste); 
          y += 7;
          }
          
      for(int y = 0; y <24; y++){ // 11
          LCD_Bitmap(32, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(32+160, y, 8, 8, Bloque_8_celeste);
          
          LCD_Bitmap(120, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(120+160, y, 8, 8, Bloque_8_morado); 
          y += 7;
          }       
      break; 
 // ------------------------------------------------------------//
// ----------------------- MAPA 3 -----------------------------//
// -----------------------------------------------------------//     

    case 3:
      Posicion_inicial_munecos(3); //poner a los munecos en su posicion inicial
      Posicion_meta(3);
      Posicion_estrellas(3);
      //LCD_Bitmap(296, 224, 24, 24, J2_Abajo_Derecha);
      //LCD_Bitmap(296, 224, 24, 24, J2_Abajo_Derecha);  

      for(int y = 216; y <240; y++){ // 1
          LCD_Bitmap(0, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(160, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(152, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(152+160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }
          
      for(int y = 192; y <216; y++){ // 2
          LCD_Bitmap(0, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(160, y, 8, 8, Bloque_8_celeste);
          y += 7;
          }
          
      for(int y = 176; y <208; y++){ // 3
          LCD_Bitmap(152, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(152+160, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(64, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(64+160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }

      for(int y = 0; y <192; y++){ // 4
          LCD_Bitmap(0, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }
          
      for(int y = 136; y <176; y++){ // 5
          LCD_Bitmap(152, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(152+160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }
          
      for(int y = 88; y <136; y++){ // 6
          LCD_Bitmap(152, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(152+160, y, 8, 8, Bloque_8_celeste);
          y += 7;
          }

      for(int y = 0; y <88; y++){ // 7
          LCD_Bitmap(152, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(152+160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }
          
      for(int y = 24; y <144; y++){ // 8
          LCD_Bitmap(32, y, 8, 8, Bloque_8_celeste);
          LCD_Bitmap(32+160, y, 8, 8, Bloque_8_celeste);
          y += 7;
          }   
          
      for(int y = 112; y <144; y++){ // 9
          LCD_Bitmap(96, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(96+160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }
          
      for(int x = 64; x <160; x++){ //10
          LCD_Bitmap(x, 208, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 208, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      for(int x = 104; x <152; x++){ //11
          LCD_Bitmap(x, 136, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 136, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      for(int x = 8; x <40; x++){ //12
          LCD_Bitmap(x, 168, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 168, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x, 184, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 184, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      LCD_Bitmap(32, 176, 8, 8, Bloque_8_morado); //13   
      LCD_Bitmap(32+160, 176, 8, 8, Bloque_8_morado); 

      for(int y = 0; y <64; y++){ // 14
          LCD_Bitmap(64, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(64+160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }
          
      for(int x = 72; x <96; x++){ //15
          LCD_Bitmap(x, 56, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 56, 8, 8, Bloque_8_morado);
          x += 7;
          }
          
      for(int y = 0; y <24; y++){ // 16
          LCD_Bitmap(96, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(96+160, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(120, y, 8, 8, Bloque_8_morado);
          LCD_Bitmap(120+160, y, 8, 8, Bloque_8_morado);
          y += 7;
          }
          
      for(int x = 104; x <120; x++){ //17
          LCD_Bitmap(x, 16, 8, 8, Bloque_8_morado);
          LCD_Bitmap(x+160, 16, 8, 8, Bloque_8_morado);
          x += 7;
          }
      break;
      
// ------------------------------------------------------------//
// ----------------------- MAPA 3 -----------------------------//
// -----------------------------------------------------------//     

    case 4:
    String Str_ganador = String(1); //convertir valor INT de cantidad estrellas a STRING
    String text_ganador = "GANADOR J" + Str_ganador;
    LCD_Print(text_ganador, 70, 110, 2, 0x07FF, 0x0000);
    //String GANADOR = "Ganador";
  }
}

//---------------------------------------------------FUNCIONES LIBRERÍA-----------------------------------------------------------------------//
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
  Serial.println("Listo init");
}

//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}

//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}

//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}

//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 

//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}

//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}


void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}

//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }
  else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
    }   
   }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// FUNCION PARA LEER VALORES ASCII DE LA SD
//***************************************************************************************************************************************
void open_SD_bitmap(unsigned char Bitmap_SD[], unsigned long Size_bitmap, char* filename) {
  File myFile = SD.open(filename);     // ABRIR EL ARCHIVO 
  unsigned long i = 0;            
  char Bitmap_SD_HEX[] = {0, 0};          //SE HACE ARREGLO DE DOS NUM, PARA CADA UNA DE LAS POSICIONES
  int Pos_1, Pos_2;                     //VARIABLES DE LAS POSICIONES
  if (myFile) {                 
    do {
      Bitmap_SD_HEX[0] = myFile.read(); //LEE
      Pos_1 = ACII_to_HEX(Bitmap_SD_HEX);       //TRANSOFRMA
      Bitmap_SD_HEX[0] = myFile.read(); //LEE
      Pos_2 = ACII_to_HEX(Bitmap_SD_HEX);       //TRANSFORMA
      Bitmap_SD[i] = (Pos_1 << 4) | (Pos_2 & 0xF);  //SE CONCATENA CONCATENA
      i++;                        
    } while (i < (Size_bitmap + 1));
  }
  myFile.close();                       
}

//***************************************************************************************************************************************
// FUNCION PARA PASAR INFO DE SD A VALORES HEXADECIMALES
//***************************************************************************************************************************************
int ACII_to_HEX(char *puntero) {
  int i = 0;
  for (;;) {
    char num = *puntero;
    if (num >= '0' && num <= '9') {
      i *= 16;
      i += num - '0';
    }
    else if (num >= 'a' && num <= 'f') {
      i *= 16;
      i += (num - 'a') + 10;
    }
    else break;
    puntero++;
  }
  return i;
}


//***************************************************************************************************************************************
// Función cuando se presiona el boton izquierdo del J1 y revisa la posici[on y se mueve
//***************************************************************************************************************************************
void switch_posicion_LEFT_J1(int num_nivel){
  switch(num_nivel){
//#################### NIVEL 1 IZQUIERDA J1 ###############################//
    case 1: //NIVEL 1 MOVERSE A LA IZQUIERDA
      switch(posicion_J1){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 1 
            for(int x = 128; x >56; x = x-1){
              LCD_Sprite(x,216,24,24,J1_Abajo_Derecha,1,0,1,0);
              V_line( x + 24, 216, 24, 0x0000);
            }
            b_LEFT_J1 = LOW;
        break; //break pos 0 /LEFT/N1/J1
        
        case 1:
        break; //break pos 1 /LEFT/N1/J1

        case 2:
        break; //break pos 2 /LEFT/N1/J1
        
        case 3:
        break; //break pos 3 /LEFT/N1/J1
        
        case 4:
        break; //break pos 4 /LEFT/N1/J1
        
        case 5:
        break; //break pos 5 /LEFT/N1/J1
        
        case 6:
        break; //break pos 6 /LEFT/N1/J1
        
        case 7:
        break; //break pos 7 /LEFT/N1/J1
        
        case 8:
        break; //break pos 8 /LEFT/N1/J1
        
        case 9:
        break; //break pos 9 /LEFT/N1/J1
        
        case 10:
        break; //break pos 10 /LEFT/N1/J1
        
        case 11:
        break; //break pos 11 /LEFT/N1/J1
        
        case 12:
        break; //break pos 12 /LEFT/N1/J1
        
        case 13:
        break; //break pos 13 /LEFT/N1/J1
        
        case 14:
        break; //break pos 14 /LEFT/N1/J1
        
        case 15:
        break; //break pos 15 /LEFT/N1/J1
        
        case 16:
        break; //break pos 16 /LEFT/N1/J1
        
        case 17:
        break; //break pos 17 /LEFT/N1/J1
        
        case 18:
        break; //break pos 18 /LEFT/N1/J1
        
        case 19:
        break; //break pos 19 /LEFT/N1/J1
        
        case 20:
        break; //break pos 20 /LEFT/N1/J1
        
        case 21:
        break; //break pos 21 /LEFT/N1/J1
        
        case 22:
        break; //break pos 22 /LEFT/N1/J1
        
        case 23:
        break; //break pos 23 /LEFT/N1/J1
        
        case 24:
        break; //break pos 24 /LEFT/N1/J1
        
        case 25:
        break; //break pos 25 /LEFT/N1/J1
        }
      
    break; //break del case 1 (nivel 1 - LEFT - J1)

//#################### NIVEL 2 IZQUIERDA J1 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - LEFT - J1)
//#################### NIVEL 3 IZQUIERDA J1 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - LEFT - J1)
    }
          
  }

  //***************************************************************************************************************************************
// Función cuando se presiona el boton izquierdo del J2 y revisa la posici[on y se mueve
//***************************************************************************************************************************************
void switch_posicion_LEFT_J2(int num_nivel){
    switch(num_nivel){
//#################### NIVEL 1 IZQUIERDA J2 ###############################//
    case 1:
      switch(posicion_J2){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 1 
          for(int x = 288; x >216; x = x-1){
            LCD_Sprite(x,216,24,24,J2_Abajo_Derecha,1,0,1,0);
            V_line( x + 24, 216, 24, 0x0000);
          }
          b_LEFT_J2 = LOW;
        break; //break pos 0 /LEFT/N1/J2
        
        case 1:
        break; //break pos 1 /LEFT/N1/J2

        case 2:
        break; //break pos 2 /LEFT/N1/J2
        
        case 3:
        break; //break pos 3 /LEFT/N1/J2
        
        case 4:
        break; //break pos 4 /LEFT/N1/J2
        
        case 5:
        break; //break pos 5 /LEFT/N1/J2
        
        case 6:
        break; //break pos 6 /LEFT/N1/J2
        
        case 7:
        break; //break pos 7 /LEFT/N1/J2
        
        case 8:
        break; //break pos 8 /LEFT/N1/J2
        
        case 9:
        break; //break pos 9 /LEFT/N1/J2
        
        case 10:
        break; //break pos 10 /LEFT/N1/J2
        
        case 11:
        break; //break pos 11 /LEFT/N1/J2
        
        case 12:
        break; //break pos 12 /LEFT/N1/J2
        
        case 13:
        break; //break pos 13 /LEFT/N1/J2
        
        case 14:
        break; //break pos 14 /LEFT/N1/J2
        
        case 15:
        break; //break pos 15 /LEFT/N1/J2
        
        case 16:
        break; //break pos 16 /LEFT/N1/J2
        
        case 17:
        break; //break pos 17 /LEFT/N1/J2
        
        case 18:
        break; //break pos 18 /LEFT/N1/J2
        
        case 19:
        break; //break pos 19 /LEFT/N1/J2
        
        case 20:
        break; //break pos 20 /LEFT/N1/J2
        
        case 21:
        break; //break pos 21 /LEFT/N1/J2
        
        case 22:
        break; //break pos 22 /LEFT/N1/J2
        
        case 23:
        break; //break pos 23 /LEFT/N1/J2
        
        case 24:
        break; //break pos 24 /LEFT/N1/J2
        
        case 25:
        break; //break pos 25 /LEFT/N1/J2
        }
    break;//break del case 1 (nivel 1 - LEFT - J2)

//#################### NIVEL 2 IZQUIERDA J2 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - LEFT - J2)
//#################### NIVEL 3 IZQUIERDA J2 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - LEFT - J2)
    }
}
//***************************************************************************************************************************************
// Función cuando se presiona el boton derecho del J1 y revisa la posicion y se mueve MAPA 1
//***************************************************************************************************************************************
void switch_posicion_RIGHT_J1(int num_nivel){
  switch(num_nivel){
//#################### NIVEL 1 DERECHA J1 ###############################//
    case 1:
      switch(posicion_J1){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 1 
        break; //break pos 0 /RIGTH/N1/J1
        
        case 1:
        break; //break pos 1 /RIGTH/N1/J1

        case 2:
        break; //break pos 2 /RIGTH/N1/J1
        
        case 3:
        break; //break pos 3 /RIGTH/N1/J1
        
        case 4:
        break; //break pos 4 /RIGTH/N1/J1
        
        case 5:
        break; //break pos 5 /RIGTH/N1/J1
        
        case 6:
        break; //break pos 6 /RIGTH/N1/J1
        
        case 7:
        break; //break pos 7 /RIGTH/N1/J1
        
        case 8:
        break; //break pos 8 /RIGTH/N1/J1
        
        case 9:
        break; //break pos 9 /RIGTH/N1/J1
        
        case 10:
        break; //break pos 10 /RIGTH/N1/J1
        
        case 11:
        break; //break pos 11 /RIGTH/N1/J1
        
        case 12:
        break; //break pos 12 /RIGTH/N1/J1
        
        case 13:
        break; //break pos 13 /RIGTH/N1/J1
        
        case 14:
        break; //break pos 14 /RIGTH/N1/J1
        
        case 15:
        break; //break pos 15 /RIGTH/N1/J1
        
        case 16:
        break; //break pos 16 /RIGTH/N1/J1
        
        case 17:
        break; //break pos 17 /RIGTH/N1/J1
        
        case 18:
        break; //break pos 18 /RIGTH/N1/J1
        
        case 19:
        break; //break pos 19 /RIGTH/N1/J1
        
        case 20:
        break; //break pos 20 /RIGTH/N1/J1
        
        case 21:
        break; //break pos 21 /RIGTH/N1/J1
        
        case 22:
        break; //break pos 22 /RIGTH/N1/J1
        
        case 23:
        break; //break pos 23 /RIGTH/N1/J1
        
        case 24:
        break; //break pos 24 /RIGTH/N1/J1
        
        case 25:
        break; //break pos 25 /RIGTH/N1
        }
    break; //break del case 1 (nivel 1 - RIGTH - J1)

//#################### NIVEL 2 DERECHA J1 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - RIGTH - J1)
//#################### NIVEL 3 DERECHA J1 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - RIGTH - J1)
    }
}

   //***************************************************************************************************************************************
// Función cuando se presiona el boton derecho del J2 y revisa la posici[on y se mueve
//***************************************************************************************************************************************
void switch_posicion_RIGHT_J2(int num_nivel){
  switch(num_nivel){
//#################### NIVEL 1 DERECHA J2 ###############################//
    case 1:
      switch(posicion_J1){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 1 
        break; //break pos 0 /RIGTH/N1/J2
        
        case 1:
        break; //break pos 1 /RIGTH/N1/J2

        case 2:
        break; //break pos 2 /RIGTH/N1/J2
        
        case 3:
        break; //break pos 3 /RIGTH/N1/J2
        
        case 4:
        break; //break pos 4 /RIGTH/N1/J2
        
        case 5:
        break; //break pos 5 /RIGTH/N1/J2
        
        case 6:
        break; //break pos 6 /RIGTH/N1/J2
        
        case 7:
        break; //break pos 7 /RIGTH/N1/J2
        
        case 8:
        break; //break pos 8 /RIGTH/N1/J2
        
        case 9:
        break; //break pos 9 /RIGTH/N1/J2
        
        case 10:
        break; //break pos 10 /RIGTH/N1/J2
        
        case 11:
        break; //break pos 11 /RIGTH/N1/J2
        
        case 12:
        break; //break pos 12 /RIGTH/N1/J2
        
        case 13:
        break; //break pos 13 /RIGTH/N1/J2
        
        case 14:
        break; //break pos 14 /RIGTH/N1/J2
        
        case 15:
        break; //break pos 15 /RIGTH/N1/J2
        
        case 16:
        break; //break pos 16 /RIGTH/N1/J2
        
        case 17:
        break; //break pos 17 /RIGTH/N1/J2
        
        case 18:
        break; //break pos 18 /RIGTH/N1/J2
        
        case 19:
        break; //break pos 19 /RIGTH/N1/J2
        
        case 20:
        break; //break pos 20 /RIGTH/N1/J2
        
        case 21:
        break; //break pos 21 /RIGTH/N1/J2
        
        case 22:
        break; //break pos 22 /RIGTH/N1/J2
        
        case 23:
        break; //break pos 23 /RIGTH/N1/J2
        
        case 24:
        break; //break pos 24 /RIGTH/N1/J2
        
        case 25:
        break; //break pos 25 /RIGTH/N1/J2
        }
    break;//break del case 1 (nivel 1 - RIGTH - J2)

//#################### NIVEL 2 DERECHA J2 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - RIGTH - J2)
//#################### NIVEL 3 DERECHA J2 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - RIGTH - J2)
    }  
} 
      
//***************************************************************************************************************************************
// Función cuando se presiona el boton arriba del J1 y revisa la posicion y se mueve 
//***************************************************************************************************************************************
void switch_posicion_UP_J1(int num_nivel){
  switch(num_nivel){
//#################### NIVEL 1 ARRIBA J1 ###############################//
    case 1:
      switch(posicion_J1){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 1 
        break; //break pos 0 /UP/N1/J1
        
        case 1:
        break; //break pos 1 /UP/N1/J1

        case 2:
        break; //break pos 2 /UP/N1/J1
        
        case 3:
        break; //break pos 3 /UP/N1/J1
        
        case 4:
        break; //break pos 4 /UP/N1/J1
        
        case 5:
        break; //break pos 5 /UP/N1/J1
        
        case 6:
        break; //break pos 6 /UP/N1/J1
        
        case 7:
        break; //break pos 7 /UP/N1/J1
        
        case 8:
        break; //break pos 8 /UP/N1/J1
        
        case 9:
        break; //break pos 9 /UP/N1/J1
        
        case 10:
        break; //break pos 10 /UP/N1/J1
        
        case 11:
        break; //break pos 11 /UP/N1/J1
        
        case 12:
        break; //break pos 12 /UP/N1/J1
        
        case 13:
        break; //break pos 13 /UP/N1/J1
        
        case 14:
        break; //break pos 14 /UP/N1/J1
        
        case 15:
        break; //break pos 15 /UP/N1/J1
        
        case 16:
        break; //break pos 16 /UP/N1/J1
        
        case 17:
        break; //break pos 17 /UP/N1/J1
        
        case 18:
        break; //break pos 18 /UP/N1/J1
        
        case 19:
        break; //break pos 19 /UP/N1/J1
        
        case 20:
        break; //break pos 20 /UP/N1/J1
        
        case 21:
        break; //break pos 21 /UP/N1/J1
        
        case 22:
        break; //break pos 22 /UP/N1/J1
        
        case 23:
        break; //break pos 23 /UP/N1/J1
        
        case 24:
        break; //break pos 24 /UP/N1/J1
        
        case 25:
        break; //break pos 25 /UP/N1/J1
        }
    break;//break del case 1 (nivel 1 - UP - J1)

//#################### NIVEL 2 ARRIBA J1 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - UP - J1)
//#################### NIVEL 3 ARRIBA J1 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - UP - J1)
    }  
}

//***************************************************************************************************************************************
// Función cuando se presiona el boton arriba del J2 y revisa la posici[on y se mueve
//***************************************************************************************************************************************
void switch_posicion_UP_J2(int num_nivel){
  switch(num_nivel){
//#################### NIVEL 1 ARRIBA J2 ###############################//
    case 1:
      switch(posicion_J1){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 2 
        break; //break pos 0 /UP/N1/J2
        
        case 1:
        break; //break pos 1 /UP/N1/J2

        case 2:
        break; //break pos 2 /UP/N1/J2
        
        case 3:
        break; //break pos 3 /UP/N1/J2
        
        case 4:
        break; //break pos 4 /UP/N1/J2
        
        case 5:
        break; //break pos 5 /UP/N1/J2
        
        case 6:
        break; //break pos 6 /UP/N1/J2
        
        case 7:
        break; //break pos 7 /UP/N1/J2
        
        case 8:
        break; //break pos 8 /UP/N1/J2
        
        case 9:
        break; //break pos 9 /UP/N1/J2
        
        case 10:
        break; //break pos 10 /UP/N1/J2
        
        case 11:
        break; //break pos 11 /UP/N1/J2
        
        case 12:
        break; //break pos 12 /UP/N1/J2
        
        case 13:
        break; //break pos 13 /UP/N1/J2
        
        case 14:
        break; //break pos 14 /UP/N1/J2
        
        case 15:
        break; //break pos 15 /UP/N1/J2
        
        case 16:
        break; //break pos 16 /UP/N1/J2
        
        case 17:
        break; //break pos 17 /UP/N1/J2
        
        case 18:
        break; //break pos 18 /UP/N1/J2
        
        case 19:
        break; //break pos 19 /UP/N1/J2
        
        case 20:
        break; //break pos 20 /UP/N1/J2
        
        case 21:
        break; //break pos 21 /UP/N1/J2
        
        case 22:
        break; //break pos 22 /UP/N1/J2
        
        case 23:
        break; //break pos 23 /UP/N1/J2
        
        case 24:
        break; //break pos 24 /UP/N1/J2
        
        case 25:
        break; //break pos 25 /UP/N1/J2
        }
    break;//break del case 1 (nivel 1 - UP - J2)

//#################### NIVEL 2 ARRIBA J2 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - UP - J2)
//#################### NIVEL 3 ARRIBA J2 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - UP - J2)
    }  
}


//***************************************************************************************************************************************
// Función cuando se presiona el boton abajo del J1 y revisa la posici[on y se mueve
//***************************************************************************************************************************************
void switch_posicion_DOWN_J1(int num_nivel){
    switch(num_nivel){
//#################### NIVEL 1 ABAJO J1 ###############################//
    case 1:
      switch(posicion_J1){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 1 
        break; //break pos 0 /DOWN/N1/J1
        
        case 1:
        break; //break pos 1 /DOWN/N1/J1

        case 2:
        break; //break pos 2 /DOWN/N1/J1
        
        case 3:
        break; //break pos 3 /DOWN/N1/J1
        
        case 4:
        break; //break pos 4 /DOWN/N1/J1
        
        case 5:
        break; //break pos 5 /DOWN/N1/J1
        
        case 6:
        break; //break pos 6 /DOWN/N1/J1
        
        case 7:
        break; //break pos 7 /DOWN/N1/J1
        
        case 8:
        break; //break pos 8 /DOWN/N1/J1
        
        case 9:
        break; //break pos 9 /DOWN/N1/J1
        
        case 10:
        break; //break pos 10 /DOWN/N1/J1
        
        case 11:
        break; //break pos 11 /DOWN/N1/J1
        
        case 12:
        break; //break pos 12 /DOWN/N1/J1
        
        case 13:
        break; //break pos 13 /DOWN/N1/J1
        
        case 14:
        break; //break pos 14 /DOWN/N1/J1
        
        case 15:
        break; //break pos 15 /DOWN/N1/J1
        
        case 16:
        break; //break pos 16 /DOWN/N1/J1
        
        case 17:
        break; //break pos 17 /DOWN/N1/J1
        
        case 18:
        break; //break pos 18 /DOWN/N1/J1
        
        case 19:
        break; //break pos 19 /DOWN/N1/J1
        
        case 20:
        break; //break pos 20 /DOWN/N1/J1
        
        case 21:
        break; //break pos 21 /DOWN/N1/J1
        
        case 22:
        break; //break pos 22 /DOWN/N1/J1
        
        case 23:
        break; //break pos 23 /DOWN/N1/J1
        
        case 24:
        break; //break pos 24 /DOWN/N1/J1
        
        case 25:
        break; //break pos 25 /DOWN/N1/J1
        }
    break;//break del case 1 (nivel 1 - DOWN - J1)

//#################### NIVEL 2 ABAJO J1 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - DOWN - J1)
//#################### NIVEL 3 ABAJO J1 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - DOWN - J1)
    }  
}

//***************************************************************************************************************************************
// Función cuando se presiona el boton abajo del J2 y revisa la posici[on y se mueve
//***************************************************************************************************************************************
void switch_posicion_DOWN_J2(int num_nivel){
  switch(num_nivel){
//#################### NIVEL 1 ABAJO J2 ###############################//
    case 1:
      switch(posicion_J1){
        case 0://POSICION INICIO NIVEL 1 JUGADOR 1 
        break; //break pos 0 /DOWN/N1/J2
        
        case 1:
        break; //break pos 1 /DOWN/N1/J2

        case 2:
        break; //break pos 2 /DOWN/N1/J2
        
        case 3:
        break; //break pos 3 /DOWN/N1/J2
        
        case 4:
        break; //break pos 4 /DOWN/N1/J2
        
        case 5:
        break; //break pos 5 /DOWN/N1/J2
        
        case 6:
        break; //break pos 6 /DOWN/N1/J2
        
        case 7:
        break; //break pos 7 /DOWN/N1/J2
        
        case 8:
        break; //break pos 8 /DOWN/N1/J2
        
        case 9:
        break; //break pos 9 /DOWN/N1/J2
        
        case 10:
        break; //break pos 10 /DOWN/N1/J2
        
        case 11:
        break; //break pos 11 /DOWN/N1/J2
        
        case 12:
        break; //break pos 12 /DOWN/N1/J2
        
        case 13:
        break; //break pos 13 /DOWN/N1/J2
        
        case 14:
        break; //break pos 14 /DOWN/N1/J2
        
        case 15:
        break; //break pos 15 /DOWN/N1/J2
        
        case 16:
        break; //break pos 16 /DOWN/N1/J2
        
        case 17:
        break; //break pos 17 /DOWN/N1/J2
        
        case 18:
        break; //break pos 18 /DOWN/N1/J2
        
        case 19:
        break; //break pos 19 /DOWN/N1/J2
        
        case 20:
        break; //break pos 20 /DOWN/N1/J2
        
        case 21:
        break; //break pos 21 /DOWN/N1/J2
        
        case 22:
        break; //break pos 22 /DOWN/N1/J2
        
        case 23:
        break; //break pos 23 /DOWN/N1/J2
        
        case 24:
        break; //break pos 24 /DOWN/N1/J2
        
        case 25:
        break; //break pos 25 /DOWN/N1/J2
        }
    break;//break del case 1 (nivel 1 - DOWN - J2)

//#################### NIVEL 2 ABAJO J2 ###############################//
    case 2:
      break;//break del case 2 (nivel 2 - DOWN - J2)
//#################### NIVEL 3 ABAJO J2 ###############################//
    case 3:
      break;//break del case 3 (nivel 3 - DOWN - J2)
    }    
}
