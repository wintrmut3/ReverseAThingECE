// for addressing, 1 is master, 2 is motor slave, and 3 is OLED slave
#include <Wire.h>

void setup()
{
    Wire.begin(3);                // join i2c bus with address #3
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);     
}

void loop()
{
    bool rs_activated;
    //any reed sw. activated, ie. AIN[0..6] is high, create a message
    //rs_activated = rsl_0 & rsl_1 & rsl_2 & rsl_3 & rsl_4 & rsl_5 & rsp_0 & rsp_1;
    if(rs_activated){
        char message[2];
        Wire.beginTransmission(9); // transmit to device #9
        //msg construction
        message[0] = rsl_0 + rsl_1 << 1 + rsl_2<< 2 + rsl_3<< 3 + rsl_4 << 4+ rsl_5 << 5+ rsp_0<< 6 + rsp_1 << 7;
        message[1] = 3 << 5; // i think; 11 << = ?
        Wire.endTransmission(); // stop transmitting
    }
}

void onReceive()
{  
    char message[2];
    int i = 0;
    while(1 < Wire.available()) // loop through all but the last
    {
        message[i] = Wire.read(); // receive byte as a character
    }

    // custom variable update here
}