//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Using the easy stepper with your arduino
//use rotate and/or rotateDeg to controll stepper motor
//speed is any number from .01 -> 1 with 1 being fastest - 
//Slower Speed == Stronger movement
/////////////////////////////////////////////////////////////////


#define DIR_PIN 2
#define STEP_PIN 3

void setup() { 
  pinMode(DIR_PIN, OUTPUT); 
  pinMode(STEP_PIN, OUTPUT); 
} 

void loop(){ 

  //rotate a specific number of degrees 
  rotateDeg(360, .5); 
  delay(1000);

  rotateDeg(-360, .1);  //reverse
  delay(1000); 

  //rotate a specific number of microsteps (8 microsteps per step)
  //a 200 step stepper would take 1600 micro steps for one full revolution
  /*rotate(1600, .5); 
  delay(1000); 

  rotate(-1600, .25); //reverse
  delay(1000);*/ 
}



void rotate(int steps, float spd){ 
  //rotate a specific number of microsteps (8 microsteps per step) - (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(DIR_PIN,dir); 

  float motorDelay = (1/spd) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(motorDelay); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(motorDelay); 
  } 
} 

void rotateDeg(float deg, float spd){ 
  //rotate a specific number of degrees (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (deg > 0)? HIGH:LOW;
  digitalWrite(DIR_PIN, dir); 

  int steps = abs(deg)*(1/0.225);
  float motorDelay = (1/spd) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(motorDelay); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(motorDelay); 
  } 
}


