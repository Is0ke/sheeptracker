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
    byte buffer[];
    int x;
    int y; 
    int z;

    /*CONSTANTS DEFINITION*/
    /*Adxl constants*/
    const byte ADXL_ADDRESS 0x53 // address of the ADXL on the GY85 chip
    const byte ADXL_POWER_CTL 0x2d // address of the power control register
    const byte ADXL_DATA_FORMAT 0x31 // address of the data format register
    const byte ADXL_DATAX0 0x32 // X0
    const byte ADXL_DATAX1 0x33 // X1
    const byte ADXL_DATAY0 0x34 // Y0
    const byte ADXL_DATAY1 0x35 // Y1
    const byte ADXL_DATAZ0 0x36 // Z0
    const byte ADXL_DATAZ1 0x37 // Z1
    const byte ADXL_REC_MODE 0x08 // Rec mode value to write in the power control
    const byte ADXL_BYTES_TO_READ 6 // two bytes for each axes
    //precision values to write in ADXL_DATA_FORMAT
    const byte ADXL_PREC2G 0x00
    const byte ADXL_PREC4G 0x01
    const byte ADXL_PREC8G 0x02
    const byte ADXL_PREC16G 0x03

    public:
    ADXL(TwoWire &tw);
    void init();
    void writeTo(byte address, byte value);
    void readData(byte buff[],int *x, int *y, int *z);
    void readFrom(byte address, size_t size, byte buff[]);
    void confPrec(byte val);
    int getX();
    int getY();
    int getZ();
    void getValues();
};

#endif