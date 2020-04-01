/*LIB INCLSUSION*/
//lib for gps
#include <SoftwareSerial.h>
#include <TinyGPS.h>
//Libs for LoRa
#include <SPI.h>
#include <LoRa.h>
//Libs for OLED display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
//time lib

//#include <ADXL.h> handmade lib for reading adxl data Work in progress
/********************************************************/
#include <ADXL.h>

/*Lora content*/
//pins
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
//specs
#define BAND 866E6
#define BANDWIDTH 125E3
#define SPREADF 7
#define SYNCWD 0x12

/*OLED Display content*/
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCR_WIDTH 128
#define SCR_HEIGHT 64


/*I2C communication ports on TTGO-LoRa-ESP32-OLED*/
#define SCL 22
#define SDA 21

#define UART_GPS 2

#define INTEL_TIME 1000
/********************************************************************/

/*GLOBAL VARS*/
int counter = 0; //count the number of packets => not used anymore TODO : remove
/*adxl axes data*/
int adxl_x = 0;
int adxl_y = 0;
int adxl_z = 0;

/*display vars*/
TwoWire scr_bus = TwoWire(0); // i2c bus to communicate with screen
Adafruit_SSD1306 display(SCR_WIDTH, SCR_HEIGHT, &scr_bus, OLED_RST);


/*accel vars*/
//byte adxl_buffer[ADXL_BYTES_TO_READ]; // byte array to store data from adxl
TwoWire adxl_bus = TwoWire(1);
ADXL adxl = ADXL(&adxl_bus);

/*GPS vars*/
//TinyGPS gps;
//SoftwareSerial ss(16,17); // might be deprecated


void setup() {

  Serial.begin(9600 );
  /*Screen init*/
  Serial.println("Resetting SSD1306");
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  /*communication on i2c bus*/
  scr_bus.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) {
    Serial.println(F("SSD1306 alloc failed ..."));
    while (1);
  }
  /*Clear display*/
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Screen init :: "); printOK();
  display.display();
  Serial.println("Screen init :: [OK]");
  delay(INTEL_TIME);

  /* Screen done
     Starting adxl */ 

  Serial.println("Starting accel");
  /*adxl initialization*/
  display.print("before the accel init");
  display.display();                                                                                                                                                                                      
  adxl.init();
  Serial.println("Accel init :: [OK]");
  display.print("Accel init :: "); printOK();
  display.display();
  delay(INTEL_TIME);
 
//  /* adxl done
//     Starting gps */
//  Serial.println("Starting GPS ...");
//  Serial2.begin(9600,SERIAL_8N1,16,17);
//  Serial.println("GPS init :: [OK]");
//  display.print("GPS init :: "); printOK();
//  display.display();

  Serial.println("Starting LoRa configuration");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  /*Check if lora init succeed or not*/
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting lora failed ...");
    display.setCursor(0, 0);
    display.print("Starting Lora :: "); printFail();
    display.display();

    while (1);
  }

  /*Configure LoRa*/
  LoRa.setSpreadingFactor(SPREADF);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setSyncWord(SYNCWD);
  
  Serial.println("Lora init :: [OK]");
  display.print("Lora init :: "); printOK();
  display.display();
  delay(INTEL_TIME);
}

void loop() {

  //readGPSData();
  adxl.getValues(&adxl_x,&adxl_y,&adxl_z);
  Serial.println(adxl_x);

//  counter ++;
//  testPacket();
//  refreshDisplay();
  delay(100);
}


void testPacket() {
  LoRa.beginPacket();
  LoRa.print("CA PINGUE ?!?!");
  LoRa.endPacket();
}

void refreshDisplay() {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Nombre de paquets");
  display.print("envoyes :: ");
  display.println(counter);
  display.display();
}

void readGPSData()
{
  while(Serial2.available())Serial.print((char)Serial2.read());
}
/** FUNCTION PrintOK
 * @brief : display OK on screen
 */
void printOK()
{
  display.println("[OK]");
  display.display();
}


/** FUNCTION PrintFail
 * @brief : display Fail on screen
 */
void printFail()
{
  display.println("[FAIL]");
  display.display();
}
