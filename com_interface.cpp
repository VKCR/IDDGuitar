#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "com_interface.h"

using namespace std;


void com_interface::Track2String(Track* track, char* b){
  LinkedList<Measure>* mlist = track->measures;
  string trackstring = "{";
  trackstring+=track->getID();
  trackstring+=";";
  trackstring+=track->getInstrumentName();
  trackstring+=";";
  trackstring+=track->getPartName();
  trackstring+=";";
   
  mlist = track->measures;
  
  while(mlist!=NULL){
    trackstring+= "{";
    trackstring+=to_string(mlist->getHead()->getNumber());
    trackstring+=";";
    trackstring+=to_string(mlist->getHead()->getTempo());
    trackstring+=";";
    LinkedList<Chord>* clist = mlist->getHead()->chords;
    
    while (clist!=NULL){
      trackstring+="{";
      trackstring+=to_string(clist->getHead()->getDuration());
      trackstring+=";";
      LinkedList<Note>* nlist = clist->getHead()->notes;
      
      while (nlist!=NULL){
	trackstring+="{";
	char tbuf[2];
	sprintf(tbuf, "%d", nlist->getHead()->getFret());
	if(nlist->getHead()->getFret() < 10){
	  trackstring += "0";
	  trackstring += tbuf[0];
	}
	else{
	  trackstring+= tbuf;
	}
	
	sprintf(tbuf, "%d", nlist->getHead()->getString());
	trackstring+=tbuf[0];
	
	if (nlist->getHead()->getRest())
	  trackstring += "1";
	else
	  trackstring += "0";
	
	trackstring+="}";
	
	nlist = nlist->getTail();
      }
      trackstring+="}";
      clist = clist->getTail();
    }
    trackstring+="}";
    mlist = mlist->getTail();
  }
  trackstring+="}";
  strcpy(b, trackstring.c_str());
  cout<<b<<endl;
}

void com_interface::Measure2String(Measure* m, char*b){
  string trackstring = "";
  
  trackstring+= "{";
  trackstring+=to_string(m->getNumber());
  trackstring+=";";
  trackstring+=to_string(m->getTempo());
  trackstring+=";";
  LinkedList<Chord>* clist = m->chords;
    
  while (clist!=NULL){
    trackstring+="{";
    trackstring+=to_string(clist->getHead()->getDuration());
    trackstring+=";";
    LinkedList<Note>* nlist = clist->getHead()->notes;
      
    while (nlist!=NULL){
      trackstring+="{";
      char tbuf[2];
      sprintf(tbuf, "%d", nlist->getHead()->getFret());
      if(nlist->getHead()->getFret() < 10){
	trackstring += "0";
	trackstring += tbuf[0];
      }
      else{
	trackstring+= tbuf;
      }
	
      sprintf(tbuf, "%d", nlist->getHead()->getString());
      trackstring+=tbuf[0];
	
      if (nlist->getHead()->getRest())
	trackstring += "1";
      else
	trackstring += "0";
	
      trackstring+="}";
	
      nlist = nlist->getTail();
    }
    trackstring+="}";
    clist = clist->getTail();
  }
  trackstring+="}";
  strcpy(b, trackstring.c_str());
  cout<<b<<endl;
}

