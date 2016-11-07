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
  COM->Stream(tab.tracks->getTail()->getHead(), 1, 20);
  COM->Stop();
  //COM.TransferTrack(tab.tracks->getTail()->getHead());
  //COM.TransferMeasureInfo(5,10);
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
