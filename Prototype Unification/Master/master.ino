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
    // made the choice to store lane in binary rather than one hot, open to changes though
    int laneIndex;

    if(OPCODE == 0){ // Game start
    /* To Do: (note: for OPCODE 0 and 1, no need to read data bits, just opcode itself should be sufficient to infer needed actions)

    * *****MOTOR CODE********* (AYESHA):
    * (TO ITEM MOTOR ARDUINO) Randomize & start motors (3 in each direction) 

    * ******* OLEDS ********* (NATASHA):
    * (ON MAIN ARDUINO, NO SIGNAL) Randomize and set OLEDs 

    ******** Timer Strip ******* (DIVYA):
    * (TO REED/LED SWITCH ARDUINO) Start the timer 
    * 
    * *****7 Seg***********(Avelyn?):
    * Reset Score to 0

    */

    }else if(OPCODE == 1){ // Game End
        // Reset motors to middle, timer/ OLEDS/ LEDS should freeze
        // in general loop function just have a: game on/ game off mode/ variable for loops

        /* *****MOTOR CODE********* (AYESHA):
        *   Reset motors to middle of belt and come to a stop

        ******** Timer Strip ******* (DIVYA):
        *   OPTIONAL: Initiate end game theatrics on LED strip
        *   REQ: LED can either power down at this point or do low power twinkling
        * 
        * 
        * ******* OLEDS ********* (NATASHA):
        *   OLEDS can either continue to display gifs and ocassionally shuffle or turn off altogether. 


        */
        
    }else if(OPCODE == 2){ // End of lane notification of item motor
        // This is a signal to the main arudino to increment the points
        // QUESTION: is the score being constantly written or only written at update? I think it's constant, so I think the score can just be incremented in this fucntion, and then
        // it will automatically update in next loop

        // Isolate the motor number in 3 Least Significant Bits (LSB) laneIndex in range [0,5]
        laneIndex = message[0] & 7; // 7 is 3'b111, so if you logical AND it with the message, only the lower 3 bits are saved in motorIndex
        

        // Check here whether or not the lane was good or bad and change the score accordingly
        // QUESTION: do we have a struct for that yet?

        // If bad, randomly switch two items positiveness and update OLEDS
        /********* NATASHA**********: Whenever a motor reaches the end of the lane, the code will enter the OPCODE == 2 if statement
        *   So at this point of the code you know that the end of lane has been reached, which lane it was (laneIndex) from the message
        *   The information of which lanes had a good/ bad item should be kept and set locally in your function already, so you can determine what action to take based on being a good/bad item
        *   Scoring LEDs and OLED are able to be globally accessed, so scoring will be accessing the good/bad item structs to determine.. score..

        */

    }else if(OPCODE == 3){ // Reed switch activation, only pertinent to motor unos
        // This signal goes from Reed switch arduino and is recieved by Item movement or Main arduino for their respective motors

        // If this is the Item movement motor, indexes for [0,5]
        // If this is the player movement motor, indexes for [0,1]
        laneIndex = message[0] & 7;

        /* *****MOTOR CODE********* (AYESHA):
        *   The motors have now reached the middle and should be able to be activated again by user input

        */

       // Reed switches might stay physically activated for a while ( if the magnet is still close by ), theoretically will only send reed switch signal on rising data edge (like rising clk edge)

    }else if(OPCODE == 4){// Motor direction update (from Master which processed user input to item motor)
        // NOTE BEFORE I FORGET: Player motor direction can be directly updated from  master, no interrupt needed.
        // Interpret 1 == 1, 0 == -1
        struct inputs_out main_inputs_out;
        char speeds = message[0];
        for(int i = 0; i < 6; i++){
            main_inputs_out.laneVelocity[i] = speeds & 1;
            speeds = speeds >> 1; // Take the least significant bit everytime
        } // lane.Velocity should be properly populated after this

        // update motor directions

    }







}