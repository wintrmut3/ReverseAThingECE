#include <AccelStepper.h>
#include <MultiStepper.h>

/* This code only controls one motor
 Only controls lane Velcotity
 Does not control Avatar Movement
 Things that need to be considered:
-> Initial direction for lanes to move in
-> Will Motor Actuation handle if we should flip velcocities
-> How to do Avatar Movement
-> Passing by reference instead of copies
*/

// player 2 is clockwise
// player 1 is counter clockwise

// Hitting edges
const int playerOneLimitSwitch = 13;
const int playerTwoLimitSwitch = 12;

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

inputs_out main_inputs_out;

const int dirPin = 2;
const int stepPin = 3;
const int motorSpeed = -200;  //going towards player 1

int currentMotorSpeed[6];

long int numStepsRequired = 0;

bool calibrated = false;

#define motorInterfaceType 1

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  pinMode(playerOneLimitSwitch, INPUT);
  pinMode(playerTwoLimitSwitch, INPUT);
  
  // put your setup code here, to run once:
  myStepper.setMaxSpeed(1000);
  myStepper.setSpeed(motorSpeed);     //move towards Player 1

  for(int i = 0; i < 6; i++){
    currentMotorSpeed[i] = motorSpeed;
  }
}

void calibrate_Motor(){
  if(playerOneLimitSwitch == HIGH){         // player 1 is absolute 0
    myStepper.setCurrentPosition(0);    
  }
  else{
    while(playerOneLimitSwitch != HIGH){    // not yet at other side
      myStepper.runSpeed();
    }
    myStepper.setCurrentPosition(0);
  }
  // go to player 2
  myStepper.setSpeed(-motorSpeed);
  while(playerTwoLimitSwitch != HIGH){
    myStepper.runSpeed();
  }
  numStepsRequired = myStepper.currentPosition();
  // go back to middle
  myStepper.setSpeed(motorSpeed);
  myStepper.runToNewPosition(numStepsRequired/2);           // go back to middle
  main_boardstate.itemLocs[0] = numStepsRequired / 2;       // update boardstates
  currentMotorSpeed[0] = 0;
}

board_state motor_actuation(board_state current_board_state, inputs_out current_inputs_out){
  if(current_inputs_out.laneVelocity[0] > 0 && currentMotorSpeed[0] != motorSpeed){    // lane velocity > 0 means towards player 1
    myStepper.setSpeed(motorSpeed);
  }
  if(current_inputs_out.laneVelocity[0] < 0 && currentMotorSpeed[0] != -motorSpeed){    // lane velocity < 0 means towards player 2
    myStepper.setSpeed(-motorSpeed);
  }
  myStepper.runSpeed();                                              // move at set speed
  main_boardstate.itemLocs[0] = myStepper.currentPosition();
  return main_boardstate;
}

void loop() {
  if(!calibrated){
    calibrate_Motor();
  }
  board_state final_boardstate = motor_actuation(main_boardstate, main_inputs_out);
}
