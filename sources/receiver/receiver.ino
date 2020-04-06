/**********************************************
* Programme de reception des trames LoRa
*********************************************/

#include <SPI.h>
#include <LoRa.h>

//pins
#define NSS 18
#define NRESET 14
#define DIO0 2

#define FREQ 866E6
#define SPREADF 7
#define SIGNAL_BANDWIDTH 125E3

#define SYNC_WORLD 0x12

void setup()
{
  Serial.begin(9600);
  Serial.println("LoRa start Initializing receiver...");
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

void loop()
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) 
    {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
