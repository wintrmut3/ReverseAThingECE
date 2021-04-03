#include <AccelStepper.h>
#include <MultiStepper.h>

/* This code is for one motor and can be used to simulate the behaviour of one lane.
 *  Since all the user input functions has not yet been integrated with the motor control function code, 
 *  this code uses inputs from pushbuttons as a subsitution of the input the user input function would have provided to the main motor control function.
 *  3 Pushbuttons:
 *  Pushbutton 7: limit Switch
 *  Pushbutton 8: user input function telling motor to go to player One
 *  Pushbutton 4: user input function tellling motor to to to player Two
 *  
 *  Assumptions: Motor will start at players One side at the beginning of the game(Need to modify if this is not the case)
 *  How this code operates: 
 *  Once the program runs the motor will motor will rotates towards the middle of the board(towards Player Two)
 *  Press Pushbutton 7(like we have hit the limit switch) and the motor will stop spinning(we have reached middle) 
 *  Go into game state
 *  Motor will now move towards player one/two until we reached either ends as it takes in input from pushbuttons until it reaches either end
 *  If reaches ends motor moves back to the middle -> reset state
 *  
 *  Different states: 
 *  BLUE LED: Calibration
 *  WHITE LED: Game State
 *  RED LED: Reset Mode
 *  
 *  Further consideration needed: 
 *  How does position work with increasing speed(need normalizing)
 *  
*/

const int ledPin = 13;        // calibration led
const int resetPin = 12;
const int gamePin = 11;
const int beyondPlayerOne = 10;
const int beyondPlayerTwo = 9;
int limitSwitch = 7;          // pushbutton for limit switch      
int playerOne = 8;            // pushbutton to move towards player One
int playerTwo = 4;            // pushbutton to move towards player two
int val = 0;                  // variable for reading the pin status    


// board state struct
struct board_state{
  float itemLocs;
};


board_state main_boardstate;


// inputs out struct
struct inputs_out{
  float laneVelocity;     //change motor speed based on this
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
const int motorSpeed = -400;  //going towards player 1, need to make it more dynamic

int currentMotorSpeed;
int nextMotorSpeed;

MOTOR_STATE motorState;

long int numStepsRequired = 0;
long int PlayerOneLocation = 0;
long int PlayerTwoLocation = 0;

bool calibrated = false;

#define motorInterfaceType 1

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  pinMode(ledPin, OUTPUT);  
  pinMode(resetPin, OUTPUT);
  pinMode(gamePin, OUTPUT);
  pinMode(beyondPlayerOne, OUTPUT);
  pinMode(beyondPlayerTwo, OUTPUT);
  pinMode(limitSwitch, INPUT);      
  pinMode(playerOne, INPUT);
  pinMode(playerTwo, INPUT);
  
  myStepper.setMaxSpeed(2000);
  myStepper.setSpeed(-motorSpeed);
  currentMotorSpeed = -motorSpeed;
  motorState = NOT_CALIBRATED;
}

// two variables needed for tracking states within calibrates
bool foundMiddle = false;
bool movingTowardsMiddle = 0;

void calibrate_Motor(){
  // check if limit switch high
   val = digitalRead(limitSwitch);
   
   if(!foundMiddle){  
      if(!movingTowardsMiddle){  
        myStepper.setCurrentPosition(0);
        myStepper.setSpeed(-motorSpeed);
        movingTowardsMiddle = 1;  
        /*if (movingTowardsMiddle){
          digitalWrite(ledPin, HIGH);  // turn LED ON
         }*/
        }
      else if(movingTowardsMiddle){
        // digitalWrite(ledPin, LOW);  // turn LED ON
        if(val == LOW){                                // have reached the middle
          numStepsRequired = myStepper.currentPosition();
          myStepper.setCurrentPosition(0);              // set this to absolute 0
          foundMiddle = true;
          movingTowardsMiddle = false;
          main_boardstate.itemLocs = 0;       // all items in middle
          currentMotorSpeed = 0;
          myStepper.setSpeed(0);
          calibrated = true;
          motorState = CALIBRATED_WAITING;
          PlayerOneLocation = -numStepsRequired;
          PlayerTwoLocation = numStepsRequired;
          digitalWrite(ledPin, HIGH);  // turn LED ON
          digitalWrite(gamePin, HIGH);  // turn LED ON
        }
      }
   }
}

// use limit switches as part of reset
board_state motor_actuation(board_state current_board_state, inputs_out current_inputs_out){
      main_boardstate.itemLocs = myStepper.currentPosition();
      if(motorState == CALIBRATED_WAITING){
        if(current_inputs_out.laneVelocity == 0)
          motorState = CALIBRATED_WAITING;
        else
          motorState = GAME_MODE_MOVING;
      }
      if(motorState == GAME_MODE_MOVING){
         if(current_inputs_out.laneVelocity > 0 && currentMotorSpeed != motorSpeed){    // lane velocity > 0 means towards player 1
          currentMotorSpeed = motorSpeed;
          myStepper.setSpeed(motorSpeed);
         }
         if(current_inputs_out.laneVelocity < 0 && currentMotorSpeed != -motorSpeed){    // lane velocity < 0 means towards player 2
          currentMotorSpeed = -motorSpeed;
          myStepper.setSpeed(-motorSpeed);
        } 
        if(currentMotorSpeed != 0){
          if(myStepper.currentPosition() <= PlayerOneLocation || myStepper.currentPosition() >= PlayerTwoLocation){
            
            motorState = GAME_MODE_SCORE_RESET;
            myStepper.moveTo(0);
            myStepper.setSpeed(-currentMotorSpeed);
            currentMotorSpeed = -currentMotorSpeed;
            current_inputs_out.laneVelocity = 0;
            digitalWrite(resetPin, HIGH);
            digitalWrite(gamePin, LOW);
            if(myStepper.currentPosition() >= PlayerTwoLocation){
              digitalWrite(beyondPlayerOne, LOW);
              digitalWrite(beyondPlayerTwo, HIGH);
            }
            else{
              digitalWrite(beyondPlayerOne, HIGH);
              digitalWrite(beyondPlayerTwo, LOW);
            }
          }
         }  
      }
     if(motorState == GAME_MODE_SCORE_RESET){
      current_inputs_out.laneVelocity = 0;
      if(myStepper.distanceToGo() == 0){       // use digitalRead(limitSwitch) with real limit switch
        motorState = GAME_MODE_MOVING;
        currentMotorSpeed = 0;
        myStepper.setSpeed(0);
        main_inputs_out.laneVelocity = 0;
        digitalWrite(beyondPlayerTwo, LOW);
        digitalWrite(beyondPlayerOne, LOW);
        digitalWrite(resetPin, LOW);
        digitalWrite(gamePin, HIGH);
      }
    }
    return main_boardstate;
}

int returnback = 0;
void loop() {
  if(!calibrated){
    motorState = NOT_CALIBRATED;
    calibrate_Motor();
    myStepper.runSpeed();
  }
  if(digitalRead(playerOne) == LOW && digitalRead(playerTwo) == HIGH){
    main_inputs_out.laneVelocity = -1;
  }
  if(digitalRead(playerTwo) == LOW && digitalRead(playerOne) == HIGH){
    main_inputs_out.laneVelocity = 1;
  }
  if(main_inputs_out.laneVelocity == -1){
    digitalWrite(beyondPlayerTwo, HIGH);  // turn LED ON
  }
  if(main_inputs_out.laneVelocity == 1){
    digitalWrite(beyondPlayerOne, HIGH);  // turn LED ON
  }
  if(calibrated){
    board_state final_boardstate = motor_actuation(main_boardstate, main_inputs_out);
  }
  myStepper.runSpeed();
}
