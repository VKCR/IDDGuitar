//Tab module of the project
//Contains a data structure for the Tab.
//Data structure is: list of tracks
//each track has some info, and has a list of measures
//Each measure has some info, and has a list of notes/chords/frets/whatevs
//Actually has a list of list of notes: because at some point chords might be played
//Must have easy access to measures.
//Like you read just a measure
//We only care about
//<note>
//<fret>
//<string>
//everything else is irrelevant
//Detect if there is a <rest>
//<chord/> element to indicate chord
#ifndef TAB_H
#define TAB_H
#include <iostream>

using namespace std;


//Linked list template class

template <class T> class LinkedList
{
 private:
  T* head;
  LinkedList<T>* tail;
 public:
  LinkedList(T*,LinkedList<T>*);
  T* getHead() { return head;}
  T* getLast() {
    LinkedList<T>* t = this;
    while (t->tail != NULL)
      t = t->tail;
    return t->head;
  }
  T* get(char*);
  LinkedList<T>* getTail(){ return tail;}
  T* append(T*);
  void print(){
    LinkedList<T>* t = this;
    while (t != NULL){
      t->head->print();
      t = t->tail;
    }
  }
};



//LINKEDLIST METHODS

template <class T> LinkedList<T>::LinkedList(T* h , LinkedList<T> *t){
  head = h;
  tail = t;
}

template <class T> T* LinkedList<T>::append(T* t){
  LinkedList<T>* tl = this;
  while (tl->tail!=NULL)
    tl = tl->tail;
  
  tl->tail = new LinkedList<T>(t, NULL);
  
  return t;
}

template <class T> T* LinkedList<T>::get(char*id){
  LinkedList<T>* t = this;
  while (t !=NULL){
    if (!strcmp(t->head->getID(),id))
      return t->head;
    else
      t = t->tail;
  }

  return NULL;
}


//NOTE STRUCTURE
//change the privacy settings
class Note
{
 public:
  int fret;
  int string;
  bool rest;
  Note(){fret = 0; string = 0; rest = false;}
  Note(int f, int s, bool r){fret = f; string = s; rest = r;}
  void print(){
    if (!rest)
      cout<<"Note : Fret "<<fret<<" | String "<<string<<endl;
    else
      cout<<"Rest"<<endl;
  }
};

class Chord
{
 public:
  LinkedList<Note>* notes;
  int duration;
  Chord(int);
  void print(){
    LinkedList<Note>* t = notes;
    cout<<"Chord "<< duration<<" : "<<endl;
    while(t != NULL){
      t->getHead()->print();
      t = t->getTail();
    }
  }
};

class Measure{
 private:
  int number;
 public:
  Measure(int);
  LinkedList<Chord>* chords;
  void print(){
    LinkedList<Chord>* t = chords;
    cout<<"Measure "<<number<<" : "<<endl;
    while (t != NULL){
      t->getHead()->print();
      t = t->getTail();
    }
  }
  
};

class Track{
 private:
  char* id;
  char* instrument_name;
  char* part_name;
  
 public:
  //list of measures
  LinkedList<Measure>* measures;
  
  Track(char*);
  void setInstrumentName(char*);
  void setPartName(char*);
  char* getID(){return id;}
  void print(){
    cout<<"ID : "<<id<<" | Instrument name : "<<instrument_name<<" | Part name : "<<part_name<<endl;
  }
};


class Tab{
 private:
  char* work_title;
  char* creator;
  
 public:
  //TrackList* tracks;
  LinkedList<Track>* tracks;
  void setWorkTitle(char*);
  void setCreator(char*);
  Tab();
};

#endif
