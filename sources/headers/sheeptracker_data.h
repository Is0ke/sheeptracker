#ifndef SHEEPTRACKER_DATA
#define SHEEPTRACKER_DATA

#include "Arduino.h"

struct st_data_t
{
    short int x;
    short int y;
    short int z;

    String lat;
    String longt;
    
    String state;
};

#endif