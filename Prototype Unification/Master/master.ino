// for addressing, 1 is master, 2 is motor slave, and 3 is OLED slave
#include <Wire.h>

void setup()
{
    Wire.begin(1);                // join i2c bus with address #1
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);     
}

void loop()
{

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