#include "Arduino.h"
#include "ADXL.h"
#include <Wire.h>

ADXL::ADXL(TwoWire *tw) // default constructor
{
    wire = tw != NULL ? tw : &Wire;
}

boolean ADXL::init()
{
    wire->beginTransmission(ADDR);
    wire->endTransmission();  
    if(!writeTo(DATA_FORMAT,PREC4G)) return false;
    if(!writeTo(POWER_CTL,REC_MODE)) return false;

    return true;
}

boolean ADXL::writeTo(byte address, byte value)
{
    wire->beginTransmission(ADDR);
    if(!wire->write(address)) return false;
    if(!wire->write(value)) return false;
    wire->endTransmission();
    return true;
}


void ADXL::readFrom(byte address, size_t size, byte buff[])
{
    wire->beginTransmission(ADDR);
    wire->write(address);
    wire->endTransmission();
    wire->beginTransmission(ADDR);
    wire->requestFrom(ADDR, size);

    int i = 0;
    while(wire->available())
    {
        buff[i] = wire->read();
        i++;
    }
    wire->endTransmission();
}

void ADXL::readData(byte buff[], int *x, int *y, int *z)
{
    uint8_t numbToRead = 6;
    readFrom(DATAX0, BYTES_TO_READ,buff);

    *x = (((int) buff[1]) << 8) | buff[0];
    *y = (((int) buff[3]) << 8) | buff[2];
    *z = (((int) buff[5]) << 8) | buff[4];
}

void ADXL::confPrec(byte val)
{
    writeTo(DATA_FORMAT,val);
}

int ADXL::getX()
{
    return adxlx;
}

int ADXL::getY()
{
    return adxly;
}

int ADXL::getZ()
{
    return adxlz;
}

void ADXL::getValues(int *x, int *y, int *z)
{
    readData(buffer,&adxlx, &adxly,&adxlz);
    *x = getX();
    *y = getY();
    *z = getZ();
}
