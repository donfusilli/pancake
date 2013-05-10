
//const int DELIMITER = '.';
//const int NEXT_TOKEN = 'N';

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int numPoints;
const int xDim = 5600; //total number of steps in x-direction (long)
const int yDim = 2400; // total number of steps in y-direction (short)

int linePoints[xDim][2]; //first index is point number
                         //second index: 0 = x, 1 = y of point
                         //ONLY read spots after they have been written to. Otherwise they could contain garbage

int batterX = 0;; //x coordinate of batter dispenser
int batterY = 0; //y coordinate of batter dispenser
                         
String coordString;

#define DIR_PIN_X 2
#define STEP_PIN_X 3

#define DIR_PIN_Y 5
#define STEP_PIN_Y 6

void setup(){
  Serial.begin(9600);
  pinMode(DIR_PIN_X, OUTPUT); 
  pinMode(STEP_PIN_X, OUTPUT); 
  pinMode(DIR_PIN_Y, OUTPUT); 
  pinMode(STEP_PIN_Y, OUTPUT);
  // attaches the servo on digital pin 7 to the servo object 
  myservo.attach(7);
  delay(1000);
  myservo.write(0);
}


void loop(){
  if(Serial.available() > 0){
    //Serial.println("Got something.");
    
    char c = Serial.read();
    //Serial.println(c); Serial.print('.');
    coordString += c;
    
    if(c == '.'){
      String c1 = getValue(coordString, ',', 0);
      if(c1 == "L"){
        // do stuff for line
        int x1 = getValue(coordString, ',', 1).toInt();
        int y1 = getValue(coordString, ',', 2).toInt();
        int x2 = getValue(coordString, ',', 3).toInt();
        int y2 = getValue(coordString, ',', 4).toInt();
        x1 = map(x1, 0, 800, 0, 5600);
        y1 = map(y1, 0, 600, 0, 2400);
        x2 = map(x2, 0, 800, 0, 5600);
        y2 = map(y2, 0, 600, 0, 2400);
        
        //setLinePoints(x1,y1,x2,y2); 
        // move to first point on line
        //moveToPoint(linePoints[0][0], linePoints[0][1]);
        // "turn on" batter
        myservo.write(180);
        /*
        for(int k = 1; k < numPoints; k++){
          int tempx = linePoints[k][0];
          int tempy = linePoints[k][1];
          moveToPoint(tempx, tempy);
        }   
        */
        // turn off batter
        myservo.write(0);
        
        Serial.print(x1); Serial.print('.');
        //Serial.println(y1); Serial.print('.');
        //Serial.println(x2); Serial.print('.');
        //Serial.println(y2); Serial.print('.');
            
      }
      else if(c1 == "C"){
        int x1 = getValue(coordString, ',', 1).toInt();
        int y1 = getValue(coordString, ',', 2).toInt();
        int r = getValue(coordString, ',', 3).toInt();
        x1 = map(x1, 0, 800, 0, 5600);
        y1 = map(y1, 0, 600, 0, 2400);
        // do something with circle coordinates
      }
      else{
        //do nothing, not circle or line
      }
      // reset coordString for next set of coordinates
      coordString = "";
    } 
    
  }
   delay(100);
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


// store point in linePoints array
// used in setLinePoints below
void setPoint(int pointNum, int x, int y){
  linePoints[pointNum][0] = x;
  linePoints[pointNum][1] = y; 
}

// implementation of line-drawing algorithm
void setLinePoints(int x0, int y0, int x1, int y1){
  int dx = x1-x0;
  numPoints = abs(x1-x0);
  int dy = y1-y0;
 
  int D =  2*dy - dx;
  setPoint(0,x0,y0);
  int y = y0;
  
  int pointNum = 1;
  
  for (int x = x0+1; x<= x1; x++){
    if(D > 0){
      y = y+1;
      setPoint(pointNum,x,y);
      D = D + (2*dy-2*dx);
      pointNum++;
    }
    else{
      setPoint(pointNum,x,y);
      D = D + (2*dy);
      pointNum++;
    }  
  }
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
  int xSpeed = .01; //range 0.01 slowest to 1 fastest
  int ySpeed = .01; //range 0.01 slowest to 1 fastest
  if(delX != 0){
    rotateX(delX, xSpeed);
  }
  if(delY != 0){
    rotateY(delY, ySpeed);
  }  
  batterX = x;
  batterY = y;
}
