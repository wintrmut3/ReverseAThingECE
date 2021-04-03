#include <AccelStepper.h>
#include <MultiStepper.h>

struct board_state{
  float itemLocs[6];
  float playerLocs[2];  
};


struct inputs_out{
  int currentTrackFrom[2];
  int currentTrackTo[2];
  bool moving[2];
  float laneVelocity[6];
};

board_state *ptr_board_state, main_boardstate;
inputs_out *ptr_outputs, main_inputs_output;

/* Moving one motor first */


board_state& motor_actuation(board_state *main_state_ptr, inputs_out *main_outputs_ptr){
  return *main_state_ptr;
}

bool limit_switches

void setup() {
  // put your setup code here, to run once:
  ptr_board_state = &main_boardstate;
  ptr_outputs = &main_inputs_output;
}


/* Two Components
 *  Avatar Movement -> Updating Player Location
 *  Bomb/RAT? Movement -> Updating Item Location (board state variable), Lane Velocity
 *  Updating itemLlocation and 
*/

/*
 * Need to consider adding lane Velocity to board state variable
*/
void loop() {
  // put your main code here, to run repeatedly:
  main_boardstate = motor_actuation(ptr_board_state, ptr_outputs);
}
