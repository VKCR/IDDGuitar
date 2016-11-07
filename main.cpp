#include <stdio.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include "TabReaderXML.h"
#include "com_interface_serial.h"

using namespace std;

/***TODO
IDEA:
We wait until we have N acks to push the subsequent measures


TOFINISH THIS WEEKEND:
make the transfer over serial transmission work
primitive UI
interface skeletons for the arduino 

CA CHIE DANS LA COLLE QUAND LENGTH EST PLUS GRAND QUE 10000 POUR TRANSFERER LA TRACK. MEME DES FOIS DES LENGTHS DE 0!!!
Abandonner String dans le programme arduino et passer a des arrays de caracteres. C'est mieux. Don't forget to FREE the memory of the character array if a new track is loaded

In fact implement FREE shit everywhere.


do something to synchronize the clocks of the software and of the redbear. This way the fret playing and the song playing will happen at the same time. This requires some timestamp control signals.

do a GUI over this interface. The gui should have a display of the fretboard which shows the frets, a load file button, a select track dropdown list, a play button, a stop button, a way to choose which measures to play, an indicator of the current measure.

FOR THE ARDUINO:
-receiving interface
-concrete implementation of the receiving interface over serial communication
-Skeleton of the program
-Skeleton of the laser operations functions



***/



//test function//
Measure* String2Measure(char* buf){
  char tbuf[50];
  int oldindex = 1;
  int index = 1;
  
  //get the measure number
  while(buf[index] != ';')
    index++;
  memcpy(tbuf, &buf[oldindex], index-oldindex);
  tbuf[index-oldindex] = 0;
  
  Measure *m = new Measure(atoi(tbuf));
  index++;
  oldindex = index;
  //get the measure tempo
  while(buf[index] != ';')
    index++;
  memcpy(tbuf, &buf[oldindex], index-oldindex);
  tbuf[index-oldindex] = 0;
  
  m->setTempo(atoi(tbuf));
  index++;
  oldindex = index;

  bool in_chord = false;
  bool in_note = false;

  int note_fret;
  int note_string;
  bool note_rest;
  LinkedList<Chord>* chords = NULL;
  bool end = true;
  while(end){
    char c = buf[index];
    cout<<c<<endl;
    switch(c){
    case '{':
      if(!in_chord){
	in_chord = true;
	index++;
	oldindex = index;
      }
      else if(!in_note){
	in_note = true;
	index++;
	oldindex = index;
      }
      break; //no other cases
      
    case '}':
      if(in_note){ //end of the note
	memcpy(tbuf, &buf[oldindex], 2);
	tbuf[2] = 0;
	note_fret = atoi(tbuf);
	memcpy(tbuf, &buf[oldindex+2], 1);
	tbuf[1] = 0;
	note_string = atoi(tbuf);
	memcpy(tbuf, &buf[oldindex+3], 1);
	tbuf[1] = 0;
	note_rest = (bool)atoi(tbuf);
	chords->getLast();
	if(chords->getLast()->notes == NULL){
	  chords->getLast()->notes = new LinkedList<Note>(new Note(note_fret, note_string, note_rest),NULL);
	}
	else
	  chords->getLast()->notes->append(new Note(note_fret, note_string, note_rest));

	index++;
	oldindex = index;
	in_note = false;
      }
      else if(in_chord){
	in_chord = false;
	index++;
	oldindex = index;
      }
      else{ //this is the end of the measure
	cout<<"this is the end"<<endl;
	end = false;
      }
      break;
      
    case ';': //end of the duration
      memcpy(tbuf, &buf[oldindex], index-oldindex);
      tbuf[index-oldindex] = 0;
      if (chords == NULL)
	chords = new LinkedList<Chord>(new Chord(atoi(tbuf)),NULL);
      else
	chords->append(new Chord(atoi(tbuf)));
      
      index++;
      oldindex = index;
      break;
    default:
      index++;
    }
    
  }
  m->chords = chords;
  return m;
}



int main(int argc, char** argv){
  Tab tab;
  char* filename;
  if (argc > 1)
    filename = argv[1];
  else{
    filename = new char[100];
    strcpy(filename, "eagles.xml");
  }

  TabReaderXML::ReadTabXML(filename,&tab);
  //tab.tracks->print();

  //tab.tracks->getTail()->getHead()->measures->print();
  com_interface* COM;
  COM = new com_interface_serial();
  COM->Stream(tab.tracks->getTail()->getHead(), 1, 50);
  COM->Stop();
  return 0;
}



/* Code organization
Different modules :
-UI module 
--- for the ui, maybe have the screen follow the tablature
--- the idea is to select what parts of the song to play
-Creator of the song data structure module (for now parse the xml file format)
--- this data is basically a list of packets of notes.
--- it should be possible to select only a few of the packets each time. For example to play some measure over and over again
-Communicator module with the microcontroller
--- transfer a packet of notes to play, and some additional parameters
--- "killswitch"
--- general communications features

For the microcontroller:
-talker with the computer/app
-listening loop 
-it just receives data packets and executes them
-if it receives a kill command it interrupts

 */
