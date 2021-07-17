int laneStatus[] = {1, 1, 1, 1, 1, 1}; //bad lane == 0, good lane == 1. Array indexes are like the lane numbers

/*when a bad item is captured, that bad item turns to good and a random good item turns bad. The input is which lane contains a captured
 * item and if it's a good or bad one. 
 */
void itemSwitching(int laneIndex, int badItem) {

  //if the captured item was a bad one: turn that bad one into a good one, turn random good one into bad one
  if (badItem == 0) {
    
    //generate a random lane to be switched to bad
    int randomLane = random(0, 7);

    //keep randomizing if the lane chosen is already bad
    while (laneStatus[randomLane] == 0) {
      randomLane = random(0, 7);
    }

    //assign the good item to bad
    laneStatus[randomLane] = 0;

    //assign the bad item to good
    laneStatus[laneIndex] = 1;
  }

  //if the captured item is a good one: turn that good one into a bad one, turn random bad one into good one
  else{
    
    //generate a random lane to be switched to good
    int randomLane = random(0, 7);

    //keep randomizing if the lane chosen is already good
    while (laneStatus[randomLane] == 1) {
      randomLane = random(0, 7);
    }

    //assign the bad item to be good
    laneStatus[randomLane] = 1;

    //assign the good item to be bad
    laneStatus[laneIndex] = 0;
  }
}
