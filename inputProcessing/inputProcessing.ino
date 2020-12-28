struct inputs_out {
  int currentTrackFrom[2]; //first element is player one's current track from, second element is player two's - values indicate which row
  int currentTrackTo[2]; //first element is player one's current track to, second element is player two's - values indicate which row
  bool moving[2] =  {false, false};
  float laneVelocity[6]; //motor velocities (+1 towards p1, -1 towards p2)
};

struct inputs_in {
  bool p1left = false;//move left
  bool p1right = false; //move right
  bool p1activate = false; //reverse a lane
  bool p2left  = false;
  bool p2right = false;
  bool p2activate = false;
  bool reset = false;
};
struct inputs_out playerO;
struct inputs_in playerI;

//preset rows for each player
playerO.currentTrackTo[0] = 2;
playerO.currentTrackTo[1] = 3;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  //put your main code here, to run repeatedly:
  playerO = checkInputs(playerI, playerO);
}

struct inputs_out checkInputs(struct inputs_in In, struct inputs_out Out) {

  //reset here because after motors have moved, the prior position changes to the moved one
  Out.currentTrackFrom[0] = Out.currentTrackTo[0];
  Out.currentTrackFrom[1] = Out.currentTrackTo[1];

  //player1 and player 2 have different left and rights, which is why the subtraction and addition signs are flipped

  //player 1
  if (In.p1left == true) { //move left
    if (Out.currentTrackFrom[0] > 0) {
      Out.currentTrackTo[0] = Out.currentTrackFrom[0] - 1;
      Out.moving[0] = true;
    }
    else {
      Out.moving[0] = false;
    }
  }

  else if (In.p1right == true) { //move right
    if (Out.currentTrackFrom[0] < 5) {
      Out.currentTrackTo[0] = Out.currentTrackFrom[0] + 1;
      Out.moving[0] = true;
    }
    else {
      Out.moving[0] = false;
    }
  }

  else if (In.p1activate == true) { //reverse direction
    if (Out.laneVelocity[Out.currentTrackFrom[0]] == 1) { //if the velocity of the current player's lane is positive, reverse that lane's velocity
      Out.laneVelocity[Out.currentTrackFrom[0]] = -1;
    }
    else {
      Out.laneVelocity[Out.currentTrackFrom[0]] = 1;
    }
    Out.moving[0] = false;
  }


  //player 2
  if (In.p2left == true) { //move left
    if (Out.currentTrackTo[1] < 5) {
      Out.currentTrackTo[1] = Out.currentTrackFrom[1] + 1;
      Out.moving[1] = true;
    }
    else {
      Out.moving[1] = false;
    }
  }

  else if (In.p2right == true) { //move right
    if (Out.currentTrackTo[1] > 0) {
      Out.currentTrackTo[1] = Out.currentTrackFrom[1] - 1;
      Out.moving[1] = true;
    }
    else {
      Out.moving[1] = false;
    }
  }

  else if (In.p2activate == true) { //reverse direction
    if (Out.laneVelocity[Out.currentTrackFrom[1]] == 1) {
      Out.laneVelocity[Out.currentTrackFrom[1]] = -1;
    }
    else {
      Out.laneVelocity[Out.currentTrackFrom[1]] = 1;
    }
    Out.moving[1] = false;
  }
  return Out;
}
