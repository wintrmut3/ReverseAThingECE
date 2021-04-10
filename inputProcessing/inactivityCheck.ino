int inactivityCount = 0;

struct inputs_in getInputsUnprioritized(){
  struct inputs_in inp;
  inp.p1right=processInput(echop1r,triggerp1r,GIp1r);
  inp.p1left=processInput(echop1l,triggerp1l,GIp1l);
  inp.p2left=processInput(echop2l,triggerp2l,GIp2l);
  inp.p2right=processInput(echop2r,triggerp2r,GIp2r);
  inp.p1activate=processInput(echop1a,triggerp1a,GIp1a);
  inp.p2activate=processInput(echop2a,triggerp2a,GIp2a);
  inp.reset=getReset();
  return inp;
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

/*Function that checks to see if no inputs are active for a certain amount of time
 * Assumes this is in Reece's code, probably will be called after his getInputsPrioritzied function
 * 
 * Inputs: None (struct inputs_in is global)
 * Outputs: True for it is inactive and false for it isn't.
 */
bool checkInactivity() {

  //if all inputs have no input
  if ((inp.p1right == false) && (inp.p1left == false) && (inp.p1activate == false) &&
      (inp.p2right == false) && (inp.p2left == false) && (inp.p2activate == false)) {
    inactivityCount++;
  }

  //if one input has an input
  else{
    inactivityCount = 0;
  }

  //if inactivity has gone for 50 cycles (placeholder number)
  if (inactivityCount == 50){
    
    //game is inactive, call some other reset function
    inactivityCount = 0;
    return true;
  }
  return false;
}
