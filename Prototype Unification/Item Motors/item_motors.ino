// for addressing, 1 is master, 2 is motor slave, and 3 is OLED slave
#include <Wire.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

// player 2 is clockwise
// player 1 is counter clockwise

// Limit switch in the middle
const int MiddleLimitSwitch = 13;			

// board state struct
struct board_state{
  float itemLocs[6];
  float playerLocs[2];  
};

board_state main_boardstate;

// inputs out struct
struct inputs_out{
  int currentTrackFrom[2];
  int currentTrackTo[2];
  bool moving[2];
  float laneVelocity[6];
};

typedef enum {
  NOT_CALIBRATED, 
  CALIBRATED_WAITING,
  GAME_MODE_MOVING,
  GAME_MODE_SCORE_RESET
} MOTOR_STATE;

inputs_out main_inputs_out;

struct item_motor_info{
  int dirPins[6];
  int stepPins[6];  
  int vel[6];
};

item_motor_info main_motor_info;

main_motor_info.dirPins[0] = 2;
main_motor_info.stepPins[0] = 3;
main_motor_info.dirPins[1] = 4;
main_motor_info.stepPins[1] = 5;
main_motor_info.dirPins[2] = 6;
main_motor_info.stepPins[2] = 7;
main_motor_info.dirPins[3] = 8;
main_motor_info.stepPins[3] = 9;
main_motor_info.dirPins[4] = 10;
main_motor_info.stepPins[4] = 11;
main_motor_info.dirPins[5] = 12;
main_motor_info.stepPins[5] = 13;
int positions[6];

// assigning motor pins in struct

MOTOR_STATE motorState[6];

long int numStepsRequired = 0;

bool calibrated = false;

#define motorInterfaceType 1

AccelStepper stepper1(motorInterfaceType, main_motor_info.stepPins[0], main_motor_info.dirPins[0]);
AccelStepper stepper2(motorInterfaceType, main_motor_info.stepPins[1], main_motor_info.dirPins[1]);
AccelStepper stepper3(motorInterfaceType, main_motor_info.stepPins[2], main_motor_info.dirPins[2]);
AccelStepper stepper4(motorInterfaceType, main_motor_info.stepPins[3], main_motor_info.dirPins[3]);
AccelStepper stepper5(motorInterfaceType, main_motor_info.stepPins[4], main_motor_info.dirPins[4]);
AccelStepper stepper6(motorInterfaceType, main_motor_info.stepPins[5], main_motor_info.dirPins[5]);

MultiStepper steppers;

void setup()
{
    Wire.begin(2);                // join i2c bus with address #2
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);     

    // Configure each stepper
    stepper1.setMaxSpeed(656); //speed in step/seconds, 656 steps = 2 mm 
    stepper2.setMaxSpeed(656);
    stepper3.setMaxSpeed(656);
    stepper4.setMaxSpeed(656);
    stepper5.setMaxSpeed(656);
    stepper6.setMaxSpeed(656);

    //add steppers to the MultiStepper object
    steppers.addStepper(stepper1);
    steppers.addStepper(stepper2);
    steppers.addStepper(stepper3);
    steppers.addStepper(stepper4);
    steppers.addStepper(stepper5);
    steppers.addStepper(stepper6);
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

    // Bit mask of 16'b11100000 = 224 in decimal

    char OPCODE = message[1] & 224; // Bitwise not logical and
    OPCODE = OPCODE >> 5; // Just want 3 MSB
    // custom variable update here

    if(OPCODE == 0){ 
        //game start
        
    }
}