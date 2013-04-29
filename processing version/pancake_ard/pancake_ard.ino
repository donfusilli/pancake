

const int DELIMITER = '.';
const int NEXT_TOKEN = 'N.';

int numPoints;
const int xDim = 1024; //first dim = # steps in x direction
int linePoints[xDim][2]; //first index is point number
                         //second index: 0 = x, 1 = y of point
                         //ONLY read spots after they have been written to. Otherwise they could contain garbage
                         
String coordString;

void setup(){
  Serial.begin(9600);
  setLinePoints(0,1,6,4);
  for (int i = 0; i<numPoints; i++){
     Serial.println(("("+String(linePoints[i][0]) + "," + String(linePoints[i][1]) + ")"));
  }
}


void loop(){
if(Serial.available() > 0){
    char c = Serial.read();
    coordString += c;
    if(c == '.'){
      coordString = "";
      String c1 = getValue(coordString, ',', 0);
      if(c1 == "L"){
        // do stuff for line
        int x1 = getValue(coordString, ',', 1).toInt();
        int y1 = getValue(coordString, ',', 2).toInt();
        int x2 = getValue(coordString, ',', 3).toInt();
        int y2 = getValue(coordString, ',', 4).toInt();
        
        setLinePoints(x1,y1,x2,y2);        
      }
      else if(c1 == "C"){
        // do stuff for circle
      }
      else{
        //do nothing
      }
      Serial.println(NEXT_TOKEN);
    } 
  }
  else{
    Serial.println("Nothing received.");
  }
}

void drawLine(int x1, int y1, int x2, int y2){
   //move to x1,y1
   //turn on batter
   //move to x2,y2
   //turn off batter
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


void setPoint(int pointNum, int x, int y){
  linePoints[pointNum][0] = x;
  linePoints[pointNum][1] = y; 
}

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
