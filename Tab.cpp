#include "Tab.h"
#include <string.h>


//CHORDS METHODS
Chord::Chord(int d){
  duration = d;
  notes = NULL;
}

//MEASURE METHODS
Measure::Measure(int n){
  number = n;
  chords = NULL;
}


//TRACK METHODS

void Track::setInstrumentName(char* s){
  instrument_name = new char[strlen(s)];
  strcpy(instrument_name, s);
}

void Track::setPartName(char* s){
  part_name = new char[strlen(s)];
  strcpy(part_name, s);
}

Track::Track(char* t){
  id = new char[strlen(t)];
  part_name = NULL;
  instrument_name = NULL;
  strcpy(id, t);
  measures = NULL;
}



//TAB METHODS

void Tab::setWorkTitle(char* s)
{
  work_title = new char[strlen(s)];
  strcpy(work_title, s);
}

void Tab::setCreator(char* s)
{
  creator = new char[strlen(s)];
  strcpy(creator, s);
}

Tab::Tab(){
  tracks = NULL;
  work_title = NULL;
  creator = NULL;
}
