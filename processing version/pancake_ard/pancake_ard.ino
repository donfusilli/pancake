

const int DELIMITER = '.';
const int NEXT_TOKEN = 'N.';
String coordString;

void setuo(){
  Serial.begin(9600);
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
        
        // implement Francois' "algorythm"
        // aka Bresenham's line algorithm
        int deltax = x2 - x1;
        int deltay = y2 - y1;
        int error = 0;
        
        // Assume deltax != 0 (line is not vertical),
        // Note that this division needs to be done 
        // in a way that preserves the fractional part
        int deltaerror = abs(deltay/deltax);
        int y = y1;
        
        // not sure what this is doing...
        //for x from x0 to x1
         //plot(x,y)
         //error := error + deltaerr
         //if error ≥ 0.5 then
             //y := y + 1
             //error := error - 1.0
        
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


/*Line is of form L,x1,y1,x2,y2.
Circle is of form C,x1,y1,rad.*/
void parse_data(String buff)
{
  int index;
  String type;
  String x1String;
  String y1String;
  String x2String;
  String y2String;
  String radString;
  
  //remove trailing dot
  buff = buff.substring(0,buff.length()-1);
  //look for first comma
  index = buff.indexOf(",");
  type = buff.substring(0,index);
  //remove type from buff
  buff = buff.substring(index+1);
  index = buff.indexOf(",");
  x1String = buff.substring(0,index);
  //remove x1 from buff
  buff = buff.substring(index+1);
  index = buff.indexOf(",");
  y1String = buff.substring(0,index);
  //remove y1 from buff
  buff = buff.substring(index+1);
  if (type == "L"){
    //type line
    index = buff.indexOf(",");
    x2String = buff.substring(0,index);
    //remove x2
    buff = buff.substring(index+1);
    index = buff.indexOf(",");
    y2String = buff.substring(0,index);
    drawLine(x1,y1,x2,y2);
  }
  else if (type == "C"){
    //type circle
    index = buff.indexOf(",");
    rad
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
