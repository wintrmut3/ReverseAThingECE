//int that says how many cycles the inputs will be "buffered" for to check for consistency - optimal number is around 4-6
const int BUFFER=6;
long GIp1l[BUFFER];
long GIp2l[BUFFER];
long GIp1r[BUFFER];
long GIp2r[BUFFER];
long GIp1a[BUFFER];
long GIp2a[BUFFER];
const int resetpin=0;
const int triggerp1l=1;
const int triggerp2l=2;
const int triggerp1r=3;
const int triggerp2r=4;
const int triggerp1a=5;
const int triggerp2a=6;
const int echop1l=A0;
const int echop2l=A1;
const int echop1r=A2;
const int echop2r=A3;
const int echop1a=A4;
const int echop2a=A5;
long DIST=610;

struct inputs_in{
  bool p1left = false;
  bool p1right = false;
  bool p1activate = false;
  bool p2left  = false;
  bool p2right = false;
  bool p2activate = false;
  bool reset = false;
};

void setup() {
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

//method to call during setup to initialize the values of all the buffers
void setBuffers(){
  for(int i=0; i<BUFFER; i++){
    GIp1l[i]=0;
    GIp2l[i]=0;
    GIp1r[i]=0;
    GIp2r[i]=0;
    GIp1a[i]=0;
    GIp2a[i]=0;
  }
}
//method to call dring setup to set the correct pinmodes for all the pins
void setPins(){
  pinMode(resetpin,INPUT);
  pinMode(echop1l,INPUT);
  pinMode(echop2l,INPUT);
  pinMode(echop1r,INPUT);
  pinMode(echop2r,INPUT);
  pinMode(echop1a,INPUT);
  pinMode(echop2a,INPUT);
  pinMode(triggerp1l,OUTPUT);
  pinMode(triggerp2l,OUTPUT);
  pinMode(triggerp1r,OUTPUT);
  pinMode(triggerp2r,OUTPUT);
  pinMode(triggerp1a,OUTPUT);
  pinMode(triggerp2a,OUTPUT);
}

//Shorthand function to get an analog read from a UV sensor using the triggerpin trigger and the echopin echo
long measure(int trigger, int echo){
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
int bufferCheck(long *inputs){
  for(int i=0; i<BUFFER; i++){
    if(inputs[i]<0){
      return 1;
    }
  }
  return 0;
}

//Internal method for adding on an element to a list of longs at the end and shifting all the other elements back by 1 and phasing out the first one (so calling it with [1,2,3] and 4 would change it to [2,3,4])
//It doesn't return anything, it just mods the same values at the old addresses
//Only used to update the queue of previous inputs for each sensor
void shiftback(long *inputs, long add){
  for(int i=0; i<BUFFER-1; i++){(
    inputs[i]=inputs[i+1];
  }
  inputs[BUFFER-1]=add;
}
//Internal method for finding the last "valid" (nonnegative) input for when it doesnt pass the buffer check
long recentValidInput(long *inputs){
  long mrv=inputs[0];
  for(int i=0; i<BUFFER; i++){
    if(inputs[i]>0){
      mrv=inputs[i];
    }
  }
  return mrv;
}
//Gets, processes, and buffers the input from the UV sensor with triggerpin and echopin, stored in the list buffList
bool processInput(int echo, int trigger, long buffList[]){
  signed int duration=measure(echo,trigger);
  shiftback(buffList,duration);
  if(bufferCheck(buffList)){
    return recentValidInput(buffList)<=DIST;
  }
  return duration<=DIST;
}
//Gets the value of the reset input (assuming it's a PIR sensor)
bool getReset(){
  return digitalRead(resetpin)==HIGH;
}
//Gets the inputs while prioritizing them based on what should/shouldn't be possilbe (order of priority for reading is: reset>activate>left>right
struct inputs_in getInputsPrioritized(){
  struct inputs_in inp;
  bool reset=getReset();
  inp.reset=reset;
  bool p1a=processInput(echop1a,triggerp1a,GIp1a);
  inp.p1activate=p1a;
  if(!reset){
    if(!p1a){
    bool p1l= processInput(echop1l,triggerp1l,GIp1l);
    inp.p1left = p1l;
    if(!p1l){
      p1r=processInput(echop1r,triggerp1r,GIp1r);
      inp.p1right = p1r;
    }else{
      inp.p1right=false;
      inp.p1left=false;
    }}else{
      inp.p1right=false;
      inp.p1left=false;
    }
    bool p2a=processInput(echop2a,triggerp2a,GIp2a);
    inp.p2aactivate=p2a;
    if(!p2a){
    bool p2l=processInput(echop2l,triggerp2l,GIp2l);
    inp.p2left=p2l;
    if(!p2l){
      p2r=processInput(echop2r,triggerp2r,GIp2r);
      inp.p2right=p2r;
    }else{
      inp.p2right=false;
      inp.p2left=false;
    }
  }else{
    inp.p2right=false;
    inp.p2left=false;
  }else{
    inp.p1left=false;
    inp.p1right=false;
    inp.p1activate=false;
    inp.p2activate=false;
    inp.p2right=false;
    inp.p2left=false;
  }
  }
  return inp;
}
//Gets the struct with inputs without any internal prioritization, just hard reads the inputs
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
