//Tab reader module for the project

#ifndef TABREADERXML_H
#define TABREADERXML_H
#include <iostream>
#include "Tab.h"
using namespace std;




class TabReaderXML{
 private:
  struct LineXML{
    int nelems; //always 1 or 3
    char first_tag [200];
    char center [200];
    char second_tag [200];
    void print(){
      cout<<first_tag<<" | "<<center<<" | "<<second_tag<<endl;
    }
  };
  static int parseTagType(LineXML*);
  static void parseTagArgs(LineXML*, char*, int);
  static void parseLine(char*, LineXML*);
  
  
  enum TagType { WORK_TITLE, CREATOR, SCORE_PART, INSTRUMENT_NAME, PART_NAME, PART, MEASURE, FRET, STRING, DURATION, REST, CHORD, END_NOTE, SOUND, UNDEFINED};
  enum TagArgs { ID, TYPE, NUMBER, TEMPO};
  
 public:
  static void ReadTabXML(const char* filename, Tab* tab);
  
};

#endif
