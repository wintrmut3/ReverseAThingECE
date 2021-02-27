#include <AccelStepper.h>
#include <MultiStepper.h>

/* This code only controls one motor
 Only controls lane Velcotity
 Does not control Avatar Movement
 Things that need to be considered:
-> Still need to add code that initializes 6 motors
-> Initial direction for lanes to move in
-> Will Motor Actuation handle if we should flip velcocities
-> How to do Avatar Movement
-> Passing by reference instead of copies
*/

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

const int dirPin = 2;
const int stepPin = 3;
const int motorSpeed = -200;  //going towards player 1, need to make it more dynamic

int currentMotorSpeed[6];
MOTOR_STATE motorState[6];

long int numStepsRequired = 0;

bool calibrated = false;

#define motorInterfaceType 1

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  pinMode(MiddleLimitSwitch, INPUT);
  
  myStepper.setMaxSpeed(1000);
  myStepper.setSpeed(-motorSpeed);     //move towards Player 2

  for(int i = 0; i < 6; i++){
    currentMotorSpeed[i] = -motorSpeed;
    motorState[i] = NOT_CALIBRATED;
  }
}

bool foundMiddle = false;
bool movingTowardsMiddle = false;
void calibrate_Motor(){
  /* Limit switches are in middle(absolute 0) 
   * Start with item at as close to Player 1 as possible
   * Start rotating towards middle till limit switch is triggered
   * For sanity there could be a check if same amount of steps required to reach Player 2
   */
   if(!foundMiddle){
      if(movingTowardsMiddle == false){
        myStepper.setCurrentPosition(0);
        myStepper.runSpeed();
        movingTowardsMiddle = true;  
      }else if(movingTowardsMiddle == true){
        if(MiddleLimitSwitch == HIGH){                  // have reached the middle
          numStepsRequired = myStepper.currentPosition();
          myStepper.setCurrentPosition(0);              // set this to absolute 0
          foundMiddle = true;
          movingTowardsMiddle = false;
        }
        else
          myStepper.runSpeed();
      }
   }
   myStepper.setSpeed(motorSpeed);
   main_boardstate.itemLocs[0] = 0;       // all items in middle
   currentMotorSpeed[0] = 0;
   calibrated = true;
   motorState[0] = CALIBRATED_WAITING;
  /* ADD FOR LOOP FOR ALL MOTORS?? */
  /*
   * for(int i = 0; i < 6; i++){
   * motorState[i] = CALIBRATED_WAITING;
   * }
  */
}

board_state motor_actuation(board_state current_board_state, inputs_out current_inputs_out){
    for(int i = 0; i < 6; i++){
        main_boardstate.itemLocs[i] = myStepper.currentPosition();
        if(myStepper.currentPosition() >= numStepsRequired || myStepper.currentPosition() <= -numStepsRequired){
          motorState[0] = GAME_MODE_SCORE_RESET;
          currentMotorSpeed[0] = -currentMotorSpeed[0];
          myStepper.moveTo(0);
          myStepper.setSpeed(currentMotorSpeed[0]);
        }
        if(motorState[i] != GAME_MODE_SCORE_RESET){
          if(current_inputs_out.laneVelocity[i] > 0 && currentMotorSpeed[i] != motorSpeed){    // lane velocity > 0 means towards player 1
            currentMotorSpeed[i] = motorSpeed;
            myStepper.setSpeed(motorSpeed);
          }
          if(current_inputs_out.laneVelocity[0] < 0 && currentMotorSpeed[0] != -motorSpeed){    // lane velocity < 0 means towards player 2
            currentMotorSpeed[i] = -motorSpeed;
            myStepper.setSpeed(-motorSpeed);
          }
          motorState[i] = GAME_MODE_MOVING;
       }
       if(motorState[i] == GAME_MODE_SCORE_RESET && main_boardstate.itemLocs[0] == 0){
        motorState[i] = GAME_MODE_MOVING;
        currentMotorSpeed[0] = 0;
        myStepper.setSpeed(0);
        if( MiddleLimitSwitch != HIGH){
          // is there something wrong?
        }
      }
   }
  myStepper.runSpeed();                                              // move at set speed, how this works in prototyping
  return main_boardstate;
}

void loop() {
  if(!calibrated){
    calibrate_Motor();
  }
  if(calibrated){
    board_state final_boardstate = motor_actuation(main_boardstate, main_inputs_out);
  }
}
