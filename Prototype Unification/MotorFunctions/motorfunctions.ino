// board state struct
struct board_state{
  float itemLocs[6];
  float playerLocs[2];  
};


/*************MOTOR SETUP ************/
const uint8_t NumberOfSteppers = 6

board_state main_boardstate;

#define motorInterfaceType 1

//Declare an array of 6 steppers
AccelStepper *stepperPtrArray[NumberOfSteppers];

// Populate with motor step pins and direction pins
uint8_t MotorStepPins = []
uint8_t MotorDirPins = []

float MaxSpeed = 1000;
float Accel = 100;
float Speed = 200;
float currentMotorSpeed[6];
MOTOR_STATE motorState[6];

typedef enum {
  NOT_CALIBRATED, 
  CALIBRATED_WAITING,
  GAME_MODE_MOVING,
  GAME_MODE_SCORE_RESET
} MOTOR_STATE;

void motorSetup(){
	for (uint8_t x = 0; x < NumberOfSteppers; x++)  {
		stepperPtrArray[x] = new AccelStepper(motorInterfaceType, MotorStepPins[x], MotorDirPins[x]);
		stepperPtrArray[x]->setMaxSpeed(MaxSpeed);
		//stepperPtrArray[x]->setAcceleration(Accel);
		stepperPtrArray[x]->setSpeed(Speed);
	}
}

/***************END OF SETUP************/

/* OPCODE 0

Pass in reference to stepper motor array or access it from global?
*/
void randomizeStartMotors(){
	uint8_t direction = [1, 1, 1, -1, -1, -1]
	//shuffle direction array
	for (uint8_t x = 0; x < NumberOfSteppers; x++)  {
		if (direction[x] == -1){
			stepperPtrArray[x]->setSpeed(Speed);
			currentMotorSpeed[x] = Speed;
			motorState[x] = GAME_MODE_MOVING;
		}
		else{
			stepperPtrArray[x]->setSpeed(-Speed);
			currentMotorSpeed[x] = -Speed;
			motorState[x] = GAME_MODE_MOVING;
		}
	}
	for (uint8_t x = 0; x < NumberOfSteppers; x++){
		stepperPtrArray[x].runSpeed();
	}
}


/* OPCODE 1
   GAME END: Motors to Middle and come to a stop
*/
void resetMotors(){
	/*Get Current Item Location*/
	for (uint8_t x = 0; x < NumberOfSteppers; x++){
		 main_boardstate.itemLocs[x] = stepperPtrArray[x]->currentPosition();
	}
		
	for (uint8_t x = 0; x < NumberOfSteppers; x++){
		/* If this is the first time in this function, reverse the motor*/
		if (motorState[x] == GAME_MODE_MOVING){
			motorState[x] = GAME_MODE_SCORE_RESET;
			currentMotorSpeed[x] = -currentMotorSpeed[x];
			stepperPtrArray[x]->moveTo(0);
			stepperPtrArray[x]->setSpeed(currentMotorSpeed[x]);
		}
	}
	for (uint8_t x = 0; x < NumberOfSteppers; x++){
		stepperPtrArray[x]->runSpeed();
	}
	
}

/* OPCODE 2 End of Lane for one motor */
void resetOneMotor(int laneIndex){
	/*Get Current Item Location*/
	for (uint8_t x = 0; x < NumberOfSteppers; x++){
		 main_boardstate.itemLocs[x] = stepperPtrArray[x]->currentPosition();
	}
		
	if (motorState[laneIndex] == GAME_MODE_MOVING){
		motorState[laneIndex] = GAME_MODE_SCORE_RESET;
		currentMotorSpeed[laneIndex] = -currentMotorSpeed[x];
		stepperPtrArray[laneIndex]->moveTo(0);
		stepperPtrArray[laneIndex]->setSpeed(currentMotorSpeed[laneIndex]);
	}
	
	stepperPtrArray[laneIndex]->runSpeed();
}


/* OPCODE 3 Reset Complete wait for user input */
void motorsResetComplete(int laneIndex){
		/* Do nothing until hitting the middle */
	if(motorState[laneIndex] == GAME_MODE_SCORE_RESET){
		if(main_boardstate.itemLocs[laneIndex] == 0){
			motorState[laneIndex] = CALIBRATED_WAITING;
			currentMotorSpeed[laneIndex] = 0;
			stepperPtrArray[laneIndex]->setSpeed(0);
		}
		else{
			motorState[laneIndex] = CALIBRATED_WAITING;
			currentMotorSpeed[laneIndex] = 0;
			stepperPtrArray->setSpeed(laneIndex);
			print("Error");
		}
	}
}

/* OPCODE 5: Update Motor Directions */
void UpdateMotorDirections(struct inputs_out main_inputs_out){
	for(int i = 0; i < 6; i++){
		main_inputs_out.itemLocs[i] = stepperPtrArray[x]->currentPosition();
		// update motor directions
		if(motorState[i] != GAME_MODE_SCORE_RESET){
			if(main_inputs_out.laneVelocity[i] > 0 && currentMotorSpeed[i] != motorSpeed){    // lane velocity > 0 means towards player 1
				currentMotorSpeed[i] = motorSpeed;
				stepperPtrArray[i]->setSpeed(motorSpeed);
			}
			if(main_inputs_out.laneVelocity[0] < 0 && currentMotorSpeed[0] != -motorSpeed){    // lane velocity < 0 means towards player 2
				currentMotorSpeed[i] = -motorSpeed;
				stepperPtrArray[i]->setSpeed(-motorSpeed);
			}
			motorState[i] = GAME_MODE_MOVING;
		}
	}
	for (uint8_t x = 0; x < NumberOfSteppers; x++){
		stepperPtrArray[x]->runSpeed();
	}
	
}
