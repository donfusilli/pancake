//TODO: string to int (cords)
//      cords to motor movement

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

int string2Int(String s){
   
}
