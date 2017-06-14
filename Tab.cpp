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
  strcpy(instrument_name, s);
}

void Track::setPartName(char* s){
  strcpy(part_name, s);
}

Track::Track(char* t){
  strcpy(id, t);
  measures = NULL;
}



//TAB METHODS

void Tab::setWorkTitle(char* s)
{
  strcpy(work_title, s);
}

void Tab::setCreator(char* s)
{
  strcpy(creator, s);
}

Tab::Tab(){
  tracks = NULL;
}
