// for addressing, 1 is master, 2 is motor slave, and 3 is OLED slave
#include <Wire.h>

struct inputs_out{
  int currentTrackFrom[2];
  int currentTrackTo[2];
  bool moving[2];
  float laneVelocity[6];
};

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

    /**** KEYS AND LEGEND*****
     * use 2 bytes, i.e. 16 bits altogether
     * Most significant 3 bits will represent the Operation code or OPCODE
     * OPCODE == 0: Game start
     * OPCODE == 1: Game End
     * OPCODE == 2: End of lane notification from motor arduino
     * OPCODE == 3: Reed switch activation
     * OPCODE == 4: Motor direction update
     * 
     * NOTE: Right now all these if and checks are in this one function but we would split it up ideally?
     * i.e. not every uno needs to know how to react to Reed switch activation
     */

    // Bit mask of 16'b11100000 = 224 in decimal

    char OPCODE = message[1] & 224; // Bitwise not logical and
    OPCODE = OPCODE >> 5; // Just want 3 MSB

    if(OPCODE == 0){ // Game start
    /* To Do: (note: for OPCODE 0 and 1, no need to read data bits, just opcode itself should be sufficient to infer needed actions)
    * Randomize & start motors (3 in each direction) (TO ITEM MOTOR ARDUINO)
    * Reset Score to 0 (ON MAIN ARDUINO, NO SIGNAL)
    * Randomize and set OLEDs (ON MAIN ARDUINO, NO SIGNAL)
    * Start the timer (TO REED SWITCH ARDUINO)
    */

    }else if(OPCODE == 1){
        // Stop motors, timer/ OLEDS/ LEDS should freeze
    }else if(OPCODE == 2){ // End of lane notification
        // This is a signal to the main arudino to increment the points
        // QUESTION: is the score being constantly written or only written at update? I think it's constant, so I think the score can just be incremented in this fucntion, and then
        // it will automatically update in next loop

        // Check here whether or not the lane was good or bad and change the score accordingly
        // QUESTION: do we have a struct for that yet?

        // If bad, randomly switch two items positiveness and update OLEDS

    }else if(OPCODE == 3){ // Reed swtich activation
        // This signal goes from Reed switch arduino and is recieved by Item movement or Main arduino for their respective motors

        // Find out which is the one hot bit, is there a faster way than just using a for loop to manually check?
        
    }else if(OPCODE == 4){// Motor direction update
        // Interpret 1 == 1, 0 == -1
        struct inputs_out main_inputs_out;
        char speeds = message[0];
        for(int i = 0; i < 6; i++){
            main_inputs_out.laneVelocity[i] = speeds & 1;
            speeds = speeds >> 1; // Take the least significant bit everytime
        }

    }







}