#include "Arduino.h"
#include <Wire.h>

#ifndef ADXL_H
#define ADXL_H


#define ADXL_ADDR 0x53
#define POWER_CTL 0x2d
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

#define REC_MODE 0x08
#define BYTES_TO_READ 6

#define PREC2G 0x00
#define PREC4G 0x01
#define PREC8G 0x02
#define PREC16G 0x03

class ADXL {
    TwoWire *i2c_bus;

    public:
    ADXL(TwoWire *tw);
    void init();
    void writeTo(byte address, byte value);
    void readData(byte buff[],int *x, int *y, int *z);
    void readFrom(byte address, size_t size, byte buff[]);
    void confPrec(byte val);
};

#endif