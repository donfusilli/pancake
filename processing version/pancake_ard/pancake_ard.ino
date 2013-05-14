/*
// Arduino sketch for Pancake Machine
// @author Don Carpenter
// @author Melanie Kauffman
// @author Cathy Lu
//
*/
#include <Servo.h>

// create servo object
Servo myservo;

//total number of steps in x-direction (long)
const int xDim = 4000; 

// total number of steps in y-direction (short)
const int yDim = 2000;

// x-coordinate of batter dispenser
int batterX = 0;

//y-coordinate of batter dispenser
int batterY = 0; 

// incoming coordinate string                   
String coordString;

// pins for x-direction motor
#define DIR_PIN_X 6
#define STEP_PIN_X 7

// pins for y-direction motor
#define DIR_PIN_Y 2
#define STEP_PIN_Y 3

void setup(){
  Serial.begin(9600);
  pinMode(DIR_PIN_X, OUTPUT); 
  pinMode(STEP_PIN_X, OUTPUT); 
  pinMode(DIR_PIN_Y, OUTPUT); 
  pinMode(STEP_PIN_Y, OUTPUT);
  // attaches the servo on digital pin 9 to the servo object 
  myservo.attach(9);
  // put servo in batter-stopping position
  myservo.write(0);
  //myservo.write(180);
}


void loop(){
  if(Serial.available() > 0){
    // read next char in serial buffer
    char c = Serial.read();
    //Serial.println(c); Serial.print('.');
    coordString += c;
    
    // if we've reached the end of a coordinate string
    if(c == '.'){
      // get first character (should be "L" or "C")
      String c1 = getValue(coordString, ',', 0);
      
      // LINE
      if(c1 == "L"){
        int x1 = getValue(coordString, ',', 1).toInt();
        int y1 = getValue(coordString, ',', 2).toInt();
        int x2 = getValue(coordString, ',', 3).toInt();
        int y2 = getValue(coordString, ',', 4).toInt();
        x1 = map(x1, 0, 800, 0, xDim);
        y1 = map(y1, 0, 600, 0, yDim);
        x2 = map(x2, 0, 800, 0, xDim);
        y2 = map(y2, 0, 600, 0, yDim);
        
        drawLine(x1,y1,x2,y2); 
    
        Serial.print(x1); Serial.print('.');
      }
      
      // CIRCLE
      else if(c1 == "C"){
        int x1 = getValue(coordString, ',', 1).toInt();
        int y1 = getValue(coordString, ',', 2).toInt();
        int r = getValue(coordString, ',', 3).toInt();
        x1 = map(x1, 0, 800, 0, xDim);
        y1 = map(y1, 0, 600, 0, yDim);
        r = map(r, 0, 300, 0, 1000);
        
        drawCircle(x1, y1, r);
        
         Serial.print(x1); Serial.print('.');
      }
      else{
        //do nothing, not circle or line
      }
      // reset coordString for next set of coordinates
      coordString = "";
    }
   else if(c == '*'){
     moveToPoint(0,0);
   } 
   else{
     // your mother was a hamster and your father smelled of elder berries
   }
  }
}

// from h-ttp://stackoverflow.com/questions/9072320/arduino-split-string-into-string-array
// see alvarolb's answer
String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


// implementation of line-drawing algorithm
void drawLine(int x0, int y0, int x1, int y1){
  int minX,minY,maxX,maxY;
  
  if(x0 <= x1){
    minX = x0;
    minY = y0;
    maxX = x1;
    maxY = y1;
  }
  else{
    minX = x1;
    minY = y1;
    maxX = x0;
    maxY = y0;  
  }
  
  int dx = maxX-minX;
  int dy = maxY-minY;
  
  //move to first point 
  int D =  2*dy - dx;
  moveToPoint(minX,minY);
  int y = minY;
  
  //turn on batter
  myservo.write(180);
  delay(800);
  
  for (int x = minX+1; x<= maxX; x++){
      if(D > 0){
        y = y+1;
        moveToPoint(x,y);
        D = D + (2*dy-2*dx);
      }
      else{
        moveToPoint(x,y);
        D = D + (2*dy);
      }  
  }
  //turn off batter
  myservo.write(0);
  delay(800);
}

void drawCircle(int x, int y, int r){
  moveToPoint(x, y);
  myservo.write(180);
  delay(100*r);
  myservo.write(0);
  delay(800);
}

//rotate steps for x-direction motor
void rotateX(int steps, float spd){ 
  //rotate a specific number of microsteps (8 microsteps per step) - (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(DIR_PIN_X,dir); 

  float motorDelay = (1/spd) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN_X, HIGH); 
    delayMicroseconds(motorDelay); 

    digitalWrite(STEP_PIN_X, LOW); 
    delayMicroseconds(motorDelay); 
  } 
} 

// rotate steps for y-direction motor
void rotateY(int steps, float spd){ 
  //rotate a specific number of microsteps (8 microsteps per step) - (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(DIR_PIN_Y,dir); 

  float motorDelay = (1/spd) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN_Y, HIGH); 
    delayMicroseconds(motorDelay); 

    digitalWrite(STEP_PIN_Y, LOW); 
    delayMicroseconds(motorDelay); 
  } 
} 

// move to (x,y) based on current location
void moveToPoint(int x, int y){
  int delX = x-batterX;
  int delY = y-batterY;
  float xSpeed = .05; //range 0.01 slowest to 1 fastest
  float ySpeed = .05; //range 0.01 slowest to 1 fastest
  //Serial.println("delX " + String(delX));
  if(delX != 0){
    rotateX(delX, xSpeed);
  }
  //Serial.println("delY " + String(delY));
  if(delY != 0){
    rotateY(delY, ySpeed);
  }  
  batterX = x;
  batterY = y;
}
