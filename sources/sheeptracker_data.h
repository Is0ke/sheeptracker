#ifndef SHEEPTRACKER_DATA
#define SHEEPTRACKER_DATA

#include <arduino.h>

struct st_data
{
    short int sheepno;      //The number of the sheep
    signed int latitude;    // latitude
    signed int longitude;   //longitude
    short int heure;
    short int minute;
    short int x_axis;
    String state;

    String toString()
    {
        return "" + sheepno + ";" + 
        latitude + "," + longitude + ";" + 
        heure + ":" + minute + ";" + 
        x_axis + ";" + state;
    }
    
}

#endif