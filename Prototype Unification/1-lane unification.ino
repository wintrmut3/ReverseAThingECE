//unification of ultrasonic sensor code and motor code for 1-lane prototype

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

//const int ledPin = 13; // calibration led
//const int resetPin = 12;
//const int gamePin = 11;
//const int beyondPlayerOne = 10;
//const int beyondPlayerTwo = 9;
//int playerOne = 8;   // pushbutton to move towards player One
//int playerTwo = 4;   // pushbutton to move towards player two
//int val = 0;         // variable for reading the pin status

//int that says how many cycles the inputs will be "buffered" for to check for consistency - optimal number is around 4-6
const int BUFFER = 6;
long GIp1l[BUFFER];
long GIp2l[BUFFER];
long GIp1r[BUFFER];
long GIp2r[BUFFER];
long GIp1a[BUFFER];
long GIp2a[BUFFER];
const int resetpin = 0;
const int triggerp1l = 1;
const int triggerp2l = 2;
const int triggerp1r = 3;
const int triggerp2r = 4;
const int triggerp1a = 5;
const int triggerp2a = 6;
const int echop1l = A0;
const int echop2l = A1;
const int echop1r = A2;
const int echop2r = A3;
const int echop1a = A4;
const int echop2a = A5;
long DIST = 610;

int reedSwitch = 7; // pushbutton for reed switch

// board state struct
struct board_state
{
  float itemLocs;
};

board_state main_boardstate;

// inputs out struct
struct inputs_out
{
  float laneVelocity; //change motor speed based on this
};

struct inputs_in
{
  bool p1left = false;
  bool p1right = false;
  bool p1activate = false;
  bool p2left = false;
  bool p2right = false;
  bool p2activate = false;
  bool reset = false;
};

typedef enum
{
  NOT_CALIBRATED,
  CALIBRATED_WAITING,
  GAME_MODE_MOVING,
  GAME_MODE_SCORE_RESET
} MOTOR_STATE;

inputs_out main_inputs_out;
inputs_in main_inputs_in;

const int dirPin = 2;
const int stepPin = 3;
const int motorSpeed = -400; //going towards player 1, need to make it more dynamic

int currentMotorSpeed;
int nextMotorSpeed;

MOTOR_STATE motorState;

long int numStepsRequired = 0;
long int PlayerOneLocation = 0;
long int PlayerTwoLocation = 0;

bool calibrated = false;

#define motorInterfaceType 1

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup()
{
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

void calibrate_Motor()
{
  // check if reed switch high
  // low when not activated
  val = digitalRead(reedSwitch);

  if (!foundMiddle)
  {
    if (!movingTowardsMiddle)
    {
      myStepper.setCurrentPosition(0);
      myStepper.setSpeed(-motorSpeed);
      movingTowardsMiddle = 1;
      /*if (movingTowardsMiddle){
          digitalWrite(ledPin, HIGH);  // turn LED ON
         }*/
    }
    else if (movingTowardsMiddle)
    {
      // digitalWrite(ledPin, LOW);  // turn LED ON
      val = HIGH;
      if (val == LOW)
      { // have reached the middle
        numStepsRequired = myStepper.currentPosition();
        myStepper.setCurrentPosition(0); // set this to absolute 0
        foundMiddle = true;
        movingTowardsMiddle = false;
        main_boardstate.itemLocs = 0; // all items in middle
        currentMotorSpeed = 0;
        myStepper.setSpeed(0);
        calibrated = true;
        motorState = CALIBRATED_WAITING;
        PlayerOneLocation = -numStepsRequired;
        PlayerTwoLocation = numStepsRequired;
      }
    }
  }
}

// use limit switches as part of reset
board_state motor_actuation(board_state current_board_state, inputs_out current_inputs_out)
{
  main_boardstate.itemLocs = myStepper.currentPosition();
  if (motorState == CALIBRATED_WAITING)
  {
    if (current_inputs_out.laneVelocity == 0)
      motorState = CALIBRATED_WAITING;
    else
      motorState = GAME_MODE_MOVING;
  }
  if (motorState == GAME_MODE_MOVING)
  {
    if (current_inputs_out.laneVelocity > 0 && currentMotorSpeed != motorSpeed)
    { // lane velocity > 0 means towards player 1
      currentMotorSpeed = motorSpeed;
      myStepper.setSpeed(motorSpeed);
    }
    if (current_inputs_out.laneVelocity < 0 && currentMotorSpeed != -motorSpeed)
    { // lane velocity < 0 means towards player 2
      currentMotorSpeed = -motorSpeed;
      myStepper.setSpeed(-motorSpeed);
    }
    if (currentMotorSpeed != 0)
    {
      if (myStepper.currentPosition() <= PlayerOneLocation || myStepper.currentPosition() >= PlayerTwoLocation)
      {

        motorState = GAME_MODE_SCORE_RESET;
        myStepper.moveTo(0);
        myStepper.setSpeed(-currentMotorSpeed);
        currentMotorSpeed = -currentMotorSpeed;
        current_inputs_out.laneVelocity = 0;
      }
    }
  }
  if (motorState == GAME_MODE_SCORE_RESET)
  {
    current_inputs_out.laneVelocity = 0;
    if (myStepper.distanceToGo() == 0)
    { // use digitalRead(limitSwitch) with real limit switch
      motorState = GAME_MODE_MOVING;
      currentMotorSpeed = 0;
      myStepper.setSpeed(0);
      main_inputs_out.laneVelocity = 0;
    }
  }
  return main_boardstate;
}

int returnback = 0;
void loop()
{
  // update main_inputs_out
  main_inputs_in = getInputsPrioritized();
  in_to_out();

  if (!calibrated)
  {
    motorState = NOT_CALIBRATED;
    calibrate_Motor();
    myStepper.runSpeed();
  }
  if (calibrated)
  {
    board_state final_boardstate = motor_actuation(main_boardstate, main_inputs_out);
  }
  myStepper.runSpeed();
}

struct inputs_out in_to_out()
{
  if (main_inputs_in.p1a == true)
  {
    main_inputs_out.laneVelocity *= -1;
  }
  if (main_inputs_in.p2a == true)
  {
    main_inputs_out.laneVelocity *= -1;
  }
}

//method to call during setup to initialize the values of all the buffers
void setBuffers()
{
  for (int i = 0; i < BUFFER; i++)
  {
    GIp1l[i] = 0;
    GIp2l[i] = 0;
    GIp1r[i] = 0;
    GIp2r[i] = 0;
    GIp1a[i] = 0;
    GIp2a[i] = 0;
  }
}
//method to call dring setup to set the correct pinmodes for all the pins
void setPins()
{
  pinMode(resetpin, INPUT);
  pinMode(echop1l, INPUT);
  pinMode(echop2l, INPUT);
  pinMode(echop1r, INPUT);
  pinMode(echop2r, INPUT);
  pinMode(echop1a, INPUT);
  pinMode(echop2a, INPUT);
  pinMode(triggerp1l, OUTPUT);
  pinMode(triggerp2l, OUTPUT);
  pinMode(triggerp1r, OUTPUT);
  pinMode(triggerp2r, OUTPUT);
  pinMode(triggerp1a, OUTPUT);
  pinMode(triggerp2a, OUTPUT);
}

//Shorthand function to get an analog read from a UV sensor using the triggerpin trigger and the echopin echo
long measure(int trigger, int echo)
{
  long duration = 0;
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  return duration;
}

//Internal method; it checks if all of the recent inputs are negative and returns 1 if they are, 0 otherwise, they're just normal inputs - basically just checks if they need to be rebuffered
int bufferCheck(long *inputs)
{
  for (int i = 0; i < BUFFER; i++)
  {
    if (inputs[i] < 0)
    {
      return 1;
    }
  }
  return 0;
}

//Internal method for adding on an element to a list of longs at the end and shifting all the other elements back by 1 and phasing out the first one (so calling it with [1,2,3] and 4 would change it to [2,3,4])
//It doesn't return anything, it just mods the same values at the old addresses
//Only used to update the queue of previous inputs for each sensor
void shiftback(long *inputs, long add)
{
  for (int i = 0; i < BUFFER - 1; i++)
  {(
    inputs[i]=inputs[i+1];
  }
  inputs[BUFFER - 1] = add;
}
//Internal method for finding the last "valid" (nonnegative) input for when it doesnt pass the buffer check
long recentValidInput(long *inputs)
{
  long mrv = inputs[0];
  for (int i = 0; i < BUFFER; i++)
  {
    if (inputs[i] > 0)
    {
      mrv = inputs[i];
    }
  }
  return mrv;
}
//Gets, processes, and buffers the input from the UV sensor with triggerpin and echopin, stored in the list buffList
bool processInput(int echo, int trigger, long buffList[])
{
  signed int duration = measure(echo, trigger);
  shiftback(buffList, duration);
  if (bufferCheck(buffList))
  {
    return recentValidInput(buffList) <= DIST;
  }
  return duration <= DIST;
}
//Gets the value of the reset input (assuming it's a PIR sensor)
bool getReset()
{
  return digitalRead(resetpin) == HIGH;
}
//Gets the inputs while prioritizing them based on what should/shouldn't be possilbe (order of priority for reading is: reset>activate>left>right
struct inputs_in getInputsPrioritized()
{
  struct inputs_in inp;
  //bool reset=getReset();
  inp.reset = false;
  bool p1a = processInput(echop1a, triggerp1a, GIp1a);
  inp.p1activate = p1a;
  bool p2a = processInput(echop2a, triggerp2a, GIp2a);
  inp.p2aactivate = p2a;
  return inp;
}
//Gets the struct with inputs without any internal prioritization, just hard reads the inputs
struct inputs_in getInputsUnprioritized()
{
  struct inputs_in inp;
  inp.p1right = processInput(echop1r, triggerp1r, GIp1r);
  inp.p1left = processInput(echop1l, triggerp1l, GIp1l);
  inp.p2left = processInput(echop2l, triggerp2l, GIp2l);
  inp.p2right = processInput(echop2r, triggerp2r, GIp2r);
  inp.p1activate = processInput(echop1a, triggerp1a, GIp1a);
  inp.p2activate = processInput(echop2a, triggerp2a, GIp2a);
  inp.reset = getReset();
  return inp;
}
