//oled setting up

//1 = ramen
//2 = fish
//3 = french fries
//4 = baguette
//5 = cheese
//6 = chinese takeout

int foodItems[] = {1, 2, 3, 4, 5, 6};
int laneItems[] = {0, 0, 0, 0, 0, 0};
bool laneAssigned = false;
int laneStatus[] = {1, 1, 1, 1, 1, 1};
int num = 0;

void oledSetUp() {

  //randomizes which oled is for each lane
  for (int i = 0; i < 6; i++) {

    laneAssigned = false;

    //while one lane has no item
    while (laneAssigned == false) {

      //random oled number
      num = random(1, 7);

      //if food is available, that lane gets that food and then that food is not available in the array anymore.
      for (int k = 0; k < 6; k++) {
        if (num == foodItems[k])
          foodItems[k] = 0;
        laneItems[i] = num;
        laneAssigned = true;
        break;
      }
    }
  }

  //randomizes bad lanes
  int badItems = 0;
  int badLane = 0;

  //chooses three lanes that are bad
  while (badItems < 3) {
    badLane = random(0, 6);
    if (laneStatus[badLane] == 1) {
      laneStatus[badLane] = 0;
      badItems++;
    }
  }
}
