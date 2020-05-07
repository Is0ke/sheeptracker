#ifndef SHEEPTRACKER_DATA
#define SHEEPTRACKER_DATA

#include "Arduino.h"

struct st_data_t
{
    inline short int x;
    inline short int y;
    inline short int z;

    String lat;
    String longt;
    
    String state;
}

#endif