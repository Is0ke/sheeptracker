/*********************************************************************************
 * Programme de récuperation des données des capteurs et d'envoi des trames LoRa
 ********************************************************************************/
//accelerogyro
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>

#include <SPI.h>
#include <LoRa.h>

//time
#include <Time2.h>

//pins
#define NSS 18
#define NRESET 14
#define DIO0 2

#define FREQ 866E6
#define SPREADF 7
#define SIGNAL_BANDWIDTH 125E3

#define SYNC_WORLD 0x12

Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

int gx=0;
int gy=0;
int gz=0;

int ax=0;
int ay=0;
int az=0;

String trame;
String latitude="";
String longitude="";

unsigned long triggerDelay=0;

unsigned long nextSendData=0;

//donnée de mouvement
unsigned long dataTime[100];
bool mouvement[100]; //true: haut / false: bas
int index=0;

void setup(void)
{
  Serial.begin(9600);
  Serial2.begin(9600);

  gyro.enableAutoRange(true);
  if(!gyro.begin())
  {
    Serial.println("No gyro detected....");
    while(1);
  }
  if(!accel.begin())
  {
    Serial.println("No accel detected...");
    while(1);
  }

  //sync le time avec la date du gps
  
  Serial.println("LoRa start Initializing sender...");
  LoRa.setPins(NSS, NRESET, DIO0);
  
  if (!LoRa.begin(FREQ)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(SPREADF);
  LoRa.setSignalBandwidth(SIGNAL_BANDWIDTH);
  LoRa.setSyncWord(SYNC_WORLD);

  Serial.println("LoRa Initializing OK!");
}

void loop(void)
{
   
  getGPS();
  getGyro();
  getAccel();

  //haut
  if(triggerDelay < millis() && gx+gy+gz>1)
  {
    Serial.println("haut!");
    dataTime[index] = now();
    mouvement[index] = true;
    
    printTime();
    printGyro();
    triggerDelay = millis() + 500;
  }

  //bas
  if(triggerDelay < millis() && gx+gy+gz<-1)
  {
    Serial.println("bas !");
    dataTime[index] = now();
    mouvement[index] = false;
    printTime();
    printGyro();
    triggerDelay = millis() + 500;
  }

  //ajouter un timer pour que les trames lora s'envoie tt les 10s / le temps néssesaire
  //ajouter la detection des hauts/bas + les envoyer 
  //si gps ready
  if(nextSendData < millis() && latitude!="" && longitude!="")
  {
    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);
    
    Serial.print("gyro: "); 
    Serial.print(gx); Serial.print(", ");
    Serial.print(gy); Serial.print(", ");
    Serial.println(gz);
    Serial.print("acceleration: "); 
    Serial.print(ax); Serial.print(", ");
    Serial.print(ay); Serial.print(", ");
    Serial.println(az);
    
    senddata();

    nextSendData = second() + 10;
  }
  //delay(1000);
}

void getAccel()
{
  sensors_event_t event;
  accel.getEvent(&event);
  ax=event.acceleration.x;
  ay=event.acceleration.y;
  az=event.acceleration.z;
}

void getGyro()
{
  sensors_event_t event;
  gyro.getEvent(&event);
  gx=event.gyro.x;
  gy=event.gyro.y;
  gz=event.gyro.z;
}

void getGPS()
{
  if(Serial2.available()>0)
    {
      char recu = Serial2.read();
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
          Serial.println(latitude);
          Serial.print("Longitude: ");
          Serial.println(longitude);
        }
        //Serial.println(trame);
        trame = "";
      }
      trame += recu;
    }
}

void senddata()
{
  Serial.println("Sending packet: ");

  // send packet
  LoRa.beginPacket();
  LoRa.print(latitude); LoRa.print(" ");
  LoRa.print(longitude); LoRa.print(" ");
  
  LoRa.print(gx); LoRa.print(" ");
  LoRa.print(gy); LoRa.print(" ");
  LoRa.print(gz); LoRa.print(" ");

  LoRa.print(ax); LoRa.print(" ");
  LoRa.print(ay); LoRa.print(" ");
  LoRa.print(az); LoRa.print(" ");
  LoRa.endPacket();
}
