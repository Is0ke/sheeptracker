/*LIB INCLSUSION*/

//Libs for LoRa
#include <SPI.h>
#include <LoRa.h>
//Libs for OLED display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

//#include <ADXL.h> handmade lib for reading adxl data Work in progress
/********************************************************/
//#include <ADXL.h>

/*CONSTANTS DEFINITION*/
/*Adxl constants*/
#define ADXL_ADDRESS 0x53 // address of the ADXL on the GY85 chip
#define ADXL_POWER_CTL 0x2d // address of the power control register
#define ADXL_DATA_FORMAT 0x31 // address of the data format register
#define ADXL_DATAX0 0x32 // X0
#define ADXL_DATAX1 0x33 // X1
#define ADXL_DATAY0 0x34 // Y0
#define ADXL_DATAY1 0x35 // Y1
#define ADXL_DATAZ0 0x36 // Z0
#define ADXL_DATAZ1 0x37 // Z1
#define ADXL_REC_MODE 0x08 // Rec mode value to write in the power control
#define ADXL_BYTES_TO_READ 6 // two bytes for each axes
//precision values to write in ADXL_DATA_FORMAT
#define ADXL_PREC2G 0x00
#define ADXL_PREC4G 0x01
#define ADXL_PREC8G 0x02
#define ADXL_PREC16G 0x03

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

#define INTEL_TIME 400
/********************************************************************/


struct st_data_t
{
    short int x;
    short int y;
    short int z;

    String lat;
    String longt;
    
    String state;
};


/*GLOBAL VARS*/
int counter = 0; //count the number of packets => not used anymore TODO : remove
/*adxl axes data*/
short int adxl_x = 0;
short int adxl_y = 0;
short int adxl_z = 0;

/*accel vars*/
byte adxl_buffer[ADXL_BYTES_TO_READ]; // byte array to store data from adxl
TwoWire adxl_bus = TwoWire(1); // i2c bus to communicate with adxl


String trame = "";
String latitude;
String longitude;

st_data_t toSend;

void setup() {

  Serial.begin(9600 );
  /*Screen init*/
  /* Screen done
     Starting adxl */ 

  Serial.println("Starting accel");
  /*adxl initialization*/
  adxl_bus.begin(SDA,SCL,100000);
  adxlWriteTo(ADXL_DATA_FORMAT,ADXL_PREC4G);
  adxlWriteTo(ADXL_POWER_CTL,ADXL_REC_MODE);
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
  delay(INTEL_TIME);
}

void loop() {

  adxlReadData();
  LoRaSend();

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
    if(toSend.x >= 0 ) toSend.state = "EATING";
    else toSend.state = "NORMAL";
    LoRa.print("Axe x = ");LoRa.print(toSend.x);LoRa.print("; ");
    LoRa.print("lat. = ");LoRa.print(toSend.lat); LoRa.print(",");LoRa.print("long. = ");LoRa.print(toSend.longt);LoRa.print(";");
    LoRa.print("etat = ");LoRa.print(toSend.state);
    LoRa.endPacket();
}





















///// ADXL Function section begins                                        //////
///// List of function that allows the ESP32 to communicate with the ADXL //////

/** FUNCTION adxlWriteTo
 * @brief: Write the value val in the register of address addr
 * @param addr : the address of the register we want to write in
 * @param val : the value we want to write
 */
void adxlWriteTo(byte addr, byte val)
{
  adxl_bus.beginTransmission(ADXL_ADDRESS);
  adxl_bus.write(addr);
  adxl_bus.write(val);
  adxl_bus.endTransmission();
}

/** FUNCTION adxlReadFrom
 * @brief : Read the value from the register address addr and store it in a buffer
 * @param addr : the address we want to read from
 * @param size : the size of the data that will be read
 * @param buff : the buffer where the data will be store
 */
void adxlReadFrom(byte addr, size_t s, byte buff[])
{
  adxl_bus.beginTransmission(ADXL_ADDRESS);
  adxl_bus.write(addr);
  adxl_bus.endTransmission();
  adxl_bus.beginTransmission(ADXL_ADDRESS);
  adxl_bus.requestFrom(ADXL_ADDRESS, s);

  int i = 0;
  while(adxl_bus.available())
  {
    buff[i] = adxl_bus.read();
    i++;
  }
  adxl_bus.endTransmission();
}

/** FUNCTION adxlReadData
 * @brief: Read the data of the ADXL registers X, Y and Z
 */
void adxlReadData()
{
  uint8_t numbToRead = 6;

  short int prev_x = 0;
  short int prev_y = 0;
  short int prev_z = 0;
  
  adxlReadFrom(ADXL_DATAX0, numbToRead, adxl_buffer);
  adxl_x = (((int) adxl_buffer[1]) << 8) | adxl_buffer[0];
  adxl_y = (((int) adxl_buffer[3]) << 8) | adxl_buffer[2];
  adxl_z = (((int) adxl_buffer[5]) << 8) | adxl_buffer[4];

  if (adxl_x != prev_x || adxl_y != prev_y || adxl_z != prev_z)
  {
    Serial.print("x = ");Serial.print(adxl_x);Serial.print(", ");
    Serial.print("y = ");Serial.print(adxl_y);Serial.print(", ");
    Serial.print("z = ");Serial.println(adxl_z);
    Serial.flush();
    prev_x = adxl_x; prev_y = adxl_y; prev_z = adxl_z;


    toSend.x = adxl_x;
    toSend.y = adxl_y;
    toSend.z= adxl_z;
  }
}

/** FUNCTION adxlConfigurePrecisionLevel
 * @brief: Configure the precision level of the ADXL
 * lower value allow high sensitivity
 */
void adxlConfigurePrecisionLevel(byte val)
{
  adxlWriteTo(ADXL_DATA_FORMAT, val);
}

///// END OF ADXL functions /////

/** FUNCTION getGPS
 * @brief: Get the GPS data
 */
void getGPS()
{
  //if(Serial2.available()>0)
  if(Serial.available()>0)
    {
      //char recu = Serial2.read();
      char recu = Serial.read();
      if(recu == '$')
      {
        String msgID = trame.substring(0,6);
        if(msgID=="$GPGGA")
        {
          int count = 0;
          int i=0;
          latitude="";
          longitude="";
          while(count<5)
          {
            if(trame[i]==',')
              count++;
            else if(count==2)
              latitude += trame[i];
            else if(count==4)
              longitude += trame[i];

            i++;
          }
          Serial.print("Latitude: ");
          toSend.lat = latitude;
          Serial.println(latitude);
          Serial.print("Longitude: ");
          toSend.longt = longitude;
          Serial.println(longitude);
        }
        Serial.println(trame);
        trame = "";
      }
      trame += recu;
    }
}
