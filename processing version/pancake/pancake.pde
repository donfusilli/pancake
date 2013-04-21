import processing.serial.*;
Serial port;
int DELIMITER = '.';
int NEXT_TOKEN = 'N';

int xDim = 800;//x dimension of griddle
int yDim = 600;//y dimension of griddle
int menuDim = yDim/5;//x dimension of menu
int offset = 5;//offset for menu buttons
color c = color(0,0,255);
boolean clickedLineBox = false;
boolean overLineBox = false;
int xLineBox = xDim;
int yLineBox = 0;
boolean clickedRunBox = false;
boolean overRunBox = false;
int xRunBox = xDim;
int yRunBox = (yDim-menuDim);
boolean clickedCircleBox = false;
boolean overCircleBox = false;
int xCircleBox = xDim;
int yCircleBox = menuDim;

int backColor = 200; //color of background
boolean drawingLine;
boolean drawingCircle;
boolean sendingData;
int xLineAnchor;
int yLineAnchor;
int oldXMouse = 0;
int oldYMouse = 0;
ArrayList <Shape> shapes = new ArrayList <Shape>(); 
Line currLine;
Circle currCircle;

interface Shape{
  void display();
  String getCordString();
}

class Line implements Shape{
  int x1, y1, x2, y2;
 
  Line(int x,int y){
    x1 = x;
    y1 = y; 
  }
  void setSecondPoint(int x,int y){
    x2 = x;
    y2 = y; 
  }
  void display(){
    line(x1,y1,x2,y2);
  } 
  String getCordString(){
    return ("L,"+x1+","+y1+","+x2+","+y2+".");
  }
  int getX1(){
    return x1; 
  }
  int getX2(){
    return x2; 
  }
  int getY1(){
    return y1; 
  }
  int getY2(){
    return y2; 
  }
}

class Circle implements Shape{
  int x1,y1,rad;
 
  Circle(int x,int y){
    x1 = x;
    y1 = y;
  }
  
  void setRad(int r){
    rad = r;
  }
  
  String getCordString(){
    return ("C,"+x1+","+y1+","+rad+".");
  }
 
  void display(){
    fill(c);
    ellipse(x1,y1,rad*2,rad*2); 
  }
  
  int getX(){
    return x1;
  }
  int getY(){
    return y1;
  }
  int getRad(){
    return rad;
  }
}

void setup(){
  defaultSet();
}

void defaultSet(){
  //set up window
  size(xDim+menuDim,yDim);
  background(backColor);

  //menu bar
  stroke(0);
  noFill();
  rect(xDim,0,menuDim,yDim);
}

void lineBox(){
  //Test if cursor is over the box
  if(mouseX > xLineBox && mouseX < xLineBox+menuDim &&
     mouseY > yLineBox && mouseY < yLineBox+menuDim){
         overLineBox = true;
     }
  else{
    overLineBox = false;
  }
  if(clickedLineBox){
    fill(255,255,0);
  }
  else{
    fill(backColor); 
  }
  stroke(c);
  rect(xLineBox,yLineBox,menuDim,menuDim);
  strokeWeight(4);
  line(xLineBox+offset,offset,
    xLineBox+menuDim-offset,menuDim-offset);
  strokeWeight(1);
}

void circleBox(){
  //Test if cursor is over the box
  if (mouseX > xCircleBox && mouseX < xCircleBox+menuDim &&
     mouseY > yCircleBox && mouseY < yCircleBox+menuDim){
       overCircleBox = true;
     }
  else{
    overCircleBox = false; 
  }
  if(clickedCircleBox){
    fill(225,255,0); 
  }
  else{
    fill(backColor);
  }
  stroke(c);
  rect(xCircleBox,yCircleBox,menuDim,menuDim);
  fill(c);
  ellipse(xCircleBox+menuDim/2,yCircleBox+menuDim/2,
          menuDim/2, menuDim/2);  
}

void runBox(){
  //Test if cursor is over the box
  if(mouseX > xRunBox && mouseX < xRunBox+menuDim &&
     mouseY > yRunBox && mouseY < yRunBox+menuDim){
       overRunBox = true; 
     }
  else{
    overRunBox = false; 
  }
  if(clickedRunBox){
    fill(255,255,0); 
  }
  else{
    fill(backColor); 
  }
  int x1 = xRunBox+(int) menuDim/4;
  int y1 = yRunBox+(int) menuDim/4;
  int x2 = xRunBox+(int) menuDim/4;
  int y2 = yDim- (int)menuDim/4;
  int l = y2-y1;
  int x3 = xDim+(3*menuDim/4);
  int y3 = yDim-menuDim/2;
  stroke(1);
  rect(xRunBox,yRunBox,menuDim,menuDim);
  fill(0);
  triangle(x1,y1,x2,y2,x3,y3);
  noFill();
}

void draw(){
  defaultSet();
  lineBox();
  circleBox();
  runBox();
  stroke(c);
  
  for (int i = 0; i < shapes.size(); i++){
    Shape s = (Shape) shapes.get(i);
    s.display(); 
  }
  
  if(mousePressed && drawingLine){
    currLine.setSecondPoint(mouseX,mouseY);
    currLine.display();
  }
  if(mousePressed && drawingCircle){
    currCircle.setRad(calculateRad(mouseX,mouseY));
    currCircle.display();
  }
}

void mousePressed(){
  if(overLineBox && !clickedRunBox){
    clickedLineBox = !clickedLineBox;
    if(clickedCircleBox && clickedLineBox){
      clickedCircleBox = false;
    }
  }
  else if(clickedLineBox && !drawingLine && mouseX <= xDim){
    //start drawing line
    currLine = new Line(mouseX,mouseY);
    drawingLine = true;
  }
  else if(overCircleBox && !clickedRunBox){
    clickedCircleBox = !clickedCircleBox;
    if(clickedCircleBox && clickedLineBox){
      clickedLineBox = false;
    }  
  }
  else if(clickedCircleBox && !drawingCircle && mouseX <= xDim){
    //start drawing circle
    currCircle = new Circle(mouseX,mouseY);
    drawingCircle = true; 
  }  
  else if(overRunBox){
    clickedRunBox = true; 
    clickedLineBox = false;
    draw();
    println("sending data");
    sendData();
  }
}

void mouseDragged(){
  if(drawingLine){
    currLine.setSecondPoint(mouseX,mouseY);
    draw();
    currLine.display();   
  }
  else if(drawingCircle){
    int rad = calculateRad(mouseX,mouseY);
    currCircle.setRad(rad);
    draw();
    currCircle.display();
  } 
}

void mouseReleased(){
  if (drawingLine){
    float delY = currLine.getY1()-currLine.getY2();
    float delX = currLine.getX1()-currLine.getX2();
    float m = delY/(delX);
    float b = currLine.getY1()-(m*currLine.getX1());
    int newX = mouseX;
    int newY = mouseY;  
    
    if (newX < 0){
      newX = 0;
      newY = (int) b;
    }
    else if(newX > xDim){
      newX = xDim;
      newY = (int) (m*xDim+b);
    }
    if (newY < 0){
      newY = 0;
      newX = (int) (-1*b/m); 
    }
    else if(newY > yDim){
      newY = yDim;
      newX = (int) ((yDim-b)/m); 
    }
    currLine.setSecondPoint(newX,newY);
    shapes.add(currLine);
    drawingLine = false;
    println(currLine.getCordString());
  }
  else if(drawingCircle){
    int xLeft = currCircle.getX()-currCircle.getRad();
    int xRight = currCircle.getX()+currCircle.getRad();
    int yUp = currCircle.getY()-currCircle.getRad();
    int yDown = currCircle.getY()+currCircle.getRad();
    int newRad = calculateRad(mouseX,mouseY);
    
    if(xLeft < 0){
      //off left side of screen
      newRad = min(newRad,currCircle.getX());
    }
    else if(xRight > xDim){
      //off right side of drawing pane
      newRad = min(newRad,xDim-currCircle.getX());
    }
    if (yUp < 0){
      //off top of screen
      newRad = min(newRad,currCircle.getY());
    }
    else if(yDown > yDim){
      //off bottom of screen
      newRad = min(newRad,yDim-currCircle.getY());
    }
    if(newRad > currCircle.getX() || newRad > currCircle.getY()){
      newRad = min(currCircle.getX(), currCircle.getY()); 
    }
    currCircle.setRad(newRad);
    shapes.add(currCircle);
    drawingCircle = false;
    println(currCircle.getCordString()); 
  }
}

void sendData(){
  //send first set of data
  println(Serial.list());
  port = new Serial(this,Serial.list()[0],57600);
  Shape s = shapes.get(0);
  port.write(s.getCordString());
  //wait for response
  while((port.readStringUntil(DELIMITER)) == null);
  //send next set of data
  for(int i=1; i<shapes.size();i++){
    s = shapes.get(i);
    port.write(s.getCordString());
    while((port.readStringUntil(DELIMITER)) == null); 
  }
  clickedRunBox = false; 
}

int calculateRad(int x, int y){
  float centerX = float(currCircle.getX());
  float centerY = float(currCircle.getY());
  float delX = x - centerX;
  float delY = y - centerY;
  float rad = sqrt(sq(delX)+sq(delY));
  return int(rad);
}
