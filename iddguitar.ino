//#include <TimeLib.h>
//#include <Time.h>


#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif



//shit
double TEMPOCOEFF = 826.0/960.0;//811.0/960.0; //a tiny bit more is required


//messages
enum MESSAGES {MEASURE, STOP, START, SIZE};
int received = 0;

//measure stuff
char **MeasureBuffer;
int measurePointer = 0;
int currMeasure = 0;
int note = 0;
int sizeMeasure = 0;
int tempo = 0;
int currMeasureNum = 0;
bool start = false;
bool readingSize = false;
unsigned long second = 1000;

//laser outputs
int laser1 = D0;
int laser2 = D1;
int laser3 = D2;
int laser4 = D3;
int laser5 = D4;
int laser6 = D5;

//laser positions
int l1 = LOW;
int l2 = LOW;
int l3 = LOW;
int l4 = LOW;
int l5 = LOW;
int l6 = LOW;

//motor position
double motor;

//time related stuff;
unsigned long startTime = 0;
unsigned long chordDurationTime = 0;
unsigned long nextChordDurationTime = 0;


void updateLasers(){  
  digitalWrite(laser1, l1);
  digitalWrite(laser2, l2);
  digitalWrite(laser3, l3);
  digitalWrite(laser4, l4);
  digitalWrite(laser5, l5);
  digitalWrite(laser6, l6);
  /*if(l1==-1){
    digitalWrite(laser1, LOW);
    //Serial.println("Laser 1 Low");
  }
  else{
    digitalWrite(laser1, HIGH);
    //Serial.println("Laser 1 High");
  }
  if(l2==-1){
    //Serial.println("Laser 2 Low");
    digitalWrite(laser2, LOW);
  }
  else{
    digitalWrite(laser2, HIGH);
    //Serial.println("Laser 2 High");
  }
  if(l3==-1){
    //Serial.println("Laser 3 Low");
    digitalWrite(laser3, LOW);
  }
  else{
    digitalWrite(laser3, HIGH);
    //Serial.println("Laser 3 High");
  }
  if(l4==-1){
    //Serial.println("Laser 4 Low");
    digitalWrite(laser4, LOW);
  }
  else{
    digitalWrite(laser4, HIGH);
    //Serial.println("Laser 4 High");
  }
  if(l5==-1){
    //Serial.println("Laser 5 Low");
    digitalWrite(laser5, LOW);
  }
  else{
    digitalWrite(laser5, HIGH);
    //Serial.println("Laser 5 High");
  }
  if(l6==-1){
    //Serial.println("Laser 6 Low");
    digitalWrite(laser6, LOW);
  }
  else{
    digitalWrite(laser6, HIGH);
    //Serial.println("Laser 6 High");
  }*/
  //reset the values for the next chord
  l1=LOW;
  l2=LOW;
  l3=LOW;
  l4=LOW;
  l5=LOW;
  l6=LOW;
}

void setLaser(char* mbuf){
  char tbuf[3];
  memcpy(tbuf, &mbuf[3], 1);
  tbuf[1] = 0;
  int rest = atoi(tbuf);
  if (!rest){
    memcpy(tbuf, mbuf, 2);
    tbuf[2] = 0;
    int fretNum = atoi(tbuf);
    memcpy(tbuf, &mbuf[2], 1);
    tbuf[1] = 0;
    int stringNum = atoi(tbuf);

    switch(stringNum){
      case 1:
        l1 = HIGH; //should be fretnum
        //Serial.print("l1 ");
        //Serial.println(fretNum);
        break;
      case 2:
        l2 = HIGH;
        
        //Serial.print("l2 ");
        //Serial.println(fretNum);
        break;
      case 3:
        l3 = HIGH;
        
        //Serial.print("l3 ");
        //Serial.println(fretNum);
        break;
      case 4:
        l4 = HIGH;
        
        //Serial.print("l4 ");
        //Serial.println(fretNum);
        break;
      case 5:
        l5 = HIGH;
        
        //Serial.print("l5 ");
        //Serial.println(fretNum);
        break;
      case 6:
        l6 = HIGH;
        
        //Serial.print("l6 ");
        //Serial.println(fretNum);
        break;
    }
  }
}

int readMeasure(char*mbuf, int note){
  char tbuf[50];
  
  int oldindex = 0;
  //check in what time interval we are
  if(millis()-startTime > chordDurationTime){//second or chordDurationTime){ //the chord has ended WHY THE FUCK DOESNT THIS WORK??
    //Serial.println((unsigned long)(millis()-startTime)/100);
    startTime = millis();
    chordDurationTime = nextChordDurationTime;
    updateLasers();
    if(mbuf[note] == '}'){ 
      //send an OK OR NOT
      //Serial.print("OK");
      //Serial.println(currMeasureNum);
      nextChordDurationTime = 0;
      currMeasureNum++;
      currMeasure++;
      return 0; //reached the end
    }
    
 
    //prepare the next chord
    oldindex = note+1;
    while (mbuf[note] != ';')
      note++;
    memcpy(tbuf, &mbuf[oldindex], note-oldindex);
    tbuf[note-oldindex] = 0;
    nextChordDurationTime = ((unsigned long)atoi(tbuf)) * TEMPOCOEFF; //VERY EMPIRICAL ;
    note++;
    while(mbuf[note] != '}'){
      note++;
      setLaser(&mbuf[note]);
      note+=5;
    }
    
    note++;
    
    
  }
  else
  {
    //do nothing
  }
  return note;
}




void Measure2number(String str, char*b){
  char *buf = new char[str.length()];
  str.toCharArray(buf, str.length());
  char tbuf[50];
  int oldindex = 1;
  int index = 1;
  
  //get the measure number
  while(buf[index] != ';')
    index++;
  memcpy(tbuf, &buf[oldindex], index-oldindex);
  free(buf);
  tbuf[index-oldindex] = 0;
  strcpy(b, tbuf);
  
}


//procedures to change the level of the laser

void setup() {
  //initialize serial transmission
  Serial.begin(9600);

  //setup the pins for the lasers
  pinMode(laser1, OUTPUT);
  pinMode(laser2, OUTPUT);
  pinMode(laser3, OUTPUT);
  pinMode(laser4, OUTPUT);
  pinMode(laser5, OUTPUT);
  pinMode(laser6, OUTPUT);

}

int parseString(String str)
{
  if (str == "STOP")
    return STOP;
  else if(str == "START")
    return START;
  else if(str == "SIZE")
    return SIZE;
  else{
    if (readingSize)
      return SIZE;
    else
      return MEASURE;
  }
    
}



void dispatchStr(String str){
  char tbuf[10];
  switch (parseString(str)){
    case MEASURE:
      str.toCharArray(MeasureBuffer[measurePointer], str.length()+1); 
      measurePointer = (measurePointer + 1);
      if (currMeasureNum == 0){
        Measure2number(str,tbuf);
        currMeasureNum = atoi(tbuf);
      }
      break;
    case STOP:
      //do stuff
      break;
    case START:
      start = true;
      break;
    case SIZE:
      if(!readingSize)
        readingSize = true;
      else{
        sizeMeasure = str.toInt();
        measurePointer = 0;
        currMeasureNum = 0;
        //initialize the measure array
        MeasureBuffer = new char*[sizeMeasure];
        for (int i=0; i<sizeMeasure;i++)
          MeasureBuffer[i] = new char[500];
        readingSize = false;
      }
      break;
    default:
      break;
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available() > 0){
    String str = Serial.readString();
    Serial.println("ACKs"); //the ACK
    dispatchStr(str);
  }

  //THIS IS THE MAIN LOOP, HERE WE CONTROL THE LASERS WITH THE MEASURES
  if(start == true){   
      if(currMeasure == sizeMeasure){
        Serial.println("End of song");
        start = false;
        free(MeasureBuffer);
        //make it more robust so that I can play a song again
        sizeMeasure = 0;
        l1=LOW;
        l2=LOW;
        l3=LOW;
        l4=LOW;
        l5=LOW;
        l6=LOW;
        updateLasers();
        measurePointer = 0;
        currMeasure = 0;
        note = 0;
        sizeMeasure = 0;
        tempo = 0;
        currMeasureNum = 0;
        start = false;
        readingSize = false;
      }else{

      if(note == 0){ //modify to start off good at the next measure
      Serial.println("Measure end ");
      Serial.println(currMeasureNum - 1);
      note++;
      while(MeasureBuffer[currMeasure][note]!='{')
        note++;
    }
   note = readMeasure(MeasureBuffer[currMeasure],note);
      }
        
  }
  
}

