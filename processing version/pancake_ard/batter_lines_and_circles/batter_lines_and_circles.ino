const int xDim = 1024; //first dim = # steps in x direction
                         
int linePoints[xDim][2]; //first index is point number
                         //second index: 0 = x, 1 = y of point
                         //ONLY read spots after they have been written to. Otherwise they could contain garbage

int dx;

void setPoint(int pointNum, int x, int y){
  linePoints[pointNum][0] = x;
  linePoints[pointNum][1] = y; 
}

void setLinePoints(int x0, int y0, int x1, int y1){
  dx = x1-x0;
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

void setup(){
  Serial.begin(9600);
  setLinePoints(0,1,6,4);
  for (int i = 0; i<dx; i++){
     Serial.println(("("+String(linePoints[i][0]) + "," + String(linePoints[i][1]) + ")"));
  }
}

void loop(){
  
}
