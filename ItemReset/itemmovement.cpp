//
//  main.cpp
//  sparkMovement
//
//  Created by Divya Kaur on 2020-12-06.
//  Copyright © 2020 Divya Kaur. All rights reserved.
//

#include <iostream>

/*int main(int argc, const char * argv[]) {
    // insert code here...
    

    return 0;
}*/


struct boardstate{
    //keeps track of position of the 6 conveyor belts
    float itemLocs [6];
    float playerLocs [2];
};

struct boardstate myStruct;

int scoreP1=0; //need to get previous score
int scoreP2=0; //need to get previous score



void reset(boardstate *myStruct){
//set the item location back to the middle, in this case 5.5
    int i;
    for(i=0; i<=6; i++){
        myStruct->itemLocs[i]=5.5;
    }
    //set player location back to the first conveyor belt
    int j;
    for (j=0; j<=2; j++){
        myStruct->playerLocs[j]=1; // set back to conveyor 1 
    }
    
    
}

void changingScore (boardstate *myStruct){
//12 is arbitrary value for length of the conveyor belt can change later
//loop through itemLocs
    int i;
    for (i=0; i<=12; i++){
        if(myStruct->itemLocs[i]==0){
            scoreP1++;
            reset(myStruct);
    }
        else if(myStruct->itemLocs[i]==11){
            scoreP2++;
            reset(myStruct);
        }
    //if itemLocs[i] == 0 or 11 then that item is end of the lane
    //change the score
        
    //hello
}
}


