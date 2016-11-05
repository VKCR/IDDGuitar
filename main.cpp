#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include "TabReaderXML.h"


using namespace std;

//TODO
//change the privacy settings for the different classes


int main(int argc, char** argv){
  Tab tab;
  if (argc > 1)
    TabReaderXML::ReadTabXML(argv[1],&tab);
  else
    TabReaderXML::ReadTabXML("eagles.xml",&tab);

  tab.tracks->print();

  tab.tracks->getTail()->getHead()->measures->print();
  
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
