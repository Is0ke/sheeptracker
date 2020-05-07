/*Inclusion des bibliotheques*/

//Bibliotheques a inclure pour le gps
//#include <SoftwareSerial.h>
#include <TinyGPS.h>

//Bibloitheques a inclure pour le module lora
#include <SPI.h>
#include <LoRa.h>

#include <Wire.h>

#include <ADXL345.h>

/***************************************
//Bibloitheques pour l'ecran OLED du LORA (peut etre enleve on ne se servira pas de l'ecran
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

//Bibliotheque pour l'ecran
/*Fin de l'inclusion des bibliotheques*/
/***************************************/

/*Declaration des constantes*/
//Broches utilisees pour le module LoRa
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
//Valeur a ecrire
#define BAND 866E6
#define BANDWIDTH 125E3
#define SPREADF 7
#define SYNCWD 0x12

/******************************
//Broches utilisees pour l'ecran
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCR_WIDTH 128
#define SCR_HEIGHT 64
*******************************/

/*I2C communication ports on TTGO-LoRa-ESP32-OLED*/
#define SCL 22
#define SDA 21


#define INTEL_TIME 400 //Valeur du delai d'affichage des informations en ms
/********************************************************************/

/*GLOBAL VARS*/


/*display vars*/
//TwoWire scr_bus = TwoWire(0); // i2c bus to communicate with screen
//Adafruit_SSD1306 display(SCR_WIDTH, SCR_HEIGHT, &scr_bus, OLED_RST);


ADXL345 adxl = ADXL345();
int x=0,y=0,z=0;


String trame = "";
String latitude;
String longitude;


void setup() {

  Serial.begin(9600 );

  /* Screen done
     Starting adxl */ 

  Serial.println("Starting accel");
  adxl.powerOn();
  Serial.println("Accel init :: [OK]");
  delay(INTEL_TIME);
  

  /* adxl done
     Starting gps */
  Serial.println("Starting GPS ...");
  Serial2.begin(9600,SERIAL_8N1,16,17);
  Serial.println("GPS init :: [OK]"); 
  

  Serial.println("Starting LoRa configuration");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  /*Check if lora init succeed or not*/
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting lora failed ...");
    while (1);
  }

  /*Configure LoRa*/
  LoRa.setSpreadingFactor(SPREADF);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setSyncWord(SYNCWD);
  
  Serial.println("Lora init :: [OK]");
}

void loop() {
  adxl.readAccel(&x,&y,&z);
  Serial.println(x);
  //LoRaSend();
  delay(100);
}

void testPacket() {
  LoRa.beginPacket();
  LoRa.print("CA PINGUE ?!?!");
  LoRa.endPacket();
}

void LoRaSend()
{
    LoRa.beginPacket();
    //LoRa.print(someValue);
    LoRa.endPacket();
}
