#include "Arduino.h"
#include "ADXL.h"
#include <Wire.h>

ADXL::ADXL(TwoWire &tw) // default constructor
{
    i2c_bus = tw;
}

void ADXL::init() 
{
    i2c_bus->begin();
    writeTo(DATA_FORMAT,PREC4G);
    writeTo(POWER_CTL,REC_MODE);
}

void ADXL::writeTo(byte address, byte value)
{
    i2c_bus->beginTransmission(ADXL_ADDR);
    i2c_bus->write(address);
    i2c_bus->write(value);
    i2c_bus->endTransmission();
}

void ADXL::readData(byte buff[], int *x, int *y, int *z)
{
    uint8_t numbToRead = 6;
    readFrom(DATAX0, BYTES_TO_READ,buff);

    *x = (((int) buff[1]) << 8) | buff[0];
    *y = (((int) buff[3]) << 8) | buff[2];
    *z = (((int) buff[5]) << 8) | buff[4];
}

void ADXL::readFrom(byte address, size_t size, byte buff[])
{
    i2c_bus->beginTransmission(ADXL_ADDR);
    i2c_bus->write(address);
    i2c_bus->endTransmission();
    i2c_bus->beginTransmission(ADXL_ADDR);
    i2c_bus->requestFrom(ADXL_ADDR, size);

    int i = 0;
    while(i2c_bus->available())
    {
        buff[i] = i2c_bus->read();
        i++;
    }
    i2c_bus->endTransmission();
}

void ADXL::confPrec(byte val)
{
    writeTo(DATA_FORMAT,val);
}

int ADXL::getX()
{
    return x;
}

int ADXL::getY()
{
    return y;
}

int ADXL::getZ()
{
    return z;
}

void ADXL::getValues(int *x, int *y, int *z)
{
    this->readData(buffer,&x,&y,&z);
    *x = getX();
    *y = getY();
    *z = getZ();
}