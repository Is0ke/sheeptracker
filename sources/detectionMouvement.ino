#include <Time2.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>

Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
int gx=0;
int gy=0;
int gz=0;

int ax=0;
int ay=0;
int az=0;

unsigned long triggerDelay=0;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Data Recuperator");

  gyro.enableAutoRange(true);
  if(!gyro.begin())
  {
    Serial.println("No gyro detected....");
    while(1);
  }
  if(!accel.begin())
  {
    Serial.println("No  accel detected...");
    while(1);
  }
}

void loop(void)
{
  sensors_event_t event; 
  gyro.getEvent(&event);
  
  gx=event.gyro.x;
  gy=event.gyro.y;
  gz=event.gyro.z;

  //tester avec la valeur absolue des gxgygz

  if(triggerDelay < millis() && gx+gy+gz>1)
  {
    Serial.println("toc !");
    //enregistrer donnée
    Serial.println("haut!");
    printTime();
    printGyro();
    triggerDelay = millis() + 500;
  }

  if(triggerDelay < millis() && gx+gy+gz<-1)
  {
    Serial.println("toc !");
    //enregistrer donnée
    Serial.println("bas !");
    printTime();
    printGyro();
    triggerDelay = millis() + 500;
  }

  //plus besoin pour le moment
  //accel.getEvent(&event);
  //ax=event.acceleration.x;
  //ay=event.acceleration.y;
  //az=event.acceleration.z;

  //Serial.print("acceleration: "); 
  //Serial.print(ax); Serial.print(", ");
  //Serial.print(ay); Serial.print(", ");
  //Serial.println(az);

  delay(100);
}

void printTime()
{
  time2_t t = now();
  Serial.print("------> date: ");
  Serial.print(second(t));
  Serial.print(" ");
  Serial.print(minute(t));
  Serial.print(" ");
  Serial.println(hour(t));
}

void printGyro()
{
  Serial.print("gyro: x"); 
  Serial.print(gx); Serial.print(", y");
  Serial.print(gy); Serial.print(", z");
  Serial.println(gz);
}
