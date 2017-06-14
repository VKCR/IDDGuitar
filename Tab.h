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
  ~LinkedList(){
    delete head;
    delete tail;
  }
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
  int size(){
    int i = 0;
    LinkedList<T>* t = this;
    while (t != NULL){
      i++;
      t = t->tail;
    }
    return i;
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
class Note
{
 private:
  int fret;
  int string;
  bool rest;
 public:
  Note(){fret = 0; string = 0; rest = false;}
  Note(int f, int s, bool r){fret = f; string = s; rest = r;}
  void print(){
    if (!rest)
      cout<<"Note : Fret "<<fret<<" | String "<<string<<endl;
    else
      cout<<"Rest"<<endl;
  }
  int getFret(){return fret;}
  int getString(){return string;}
  bool getRest(){return rest;}
    
};

class Chord
{
 private:
  int duration;
 public:
  LinkedList<Note>* notes;
  Chord(int);
  ~Chord(){
    delete notes;
  }
  void print(){
    LinkedList<Note>* t = notes;
    cout<<"Chord "<< duration<<" : "<<endl;
    while(t != NULL){
      t->getHead()->print();
      t = t->getTail();
    }
  }
  int getDuration(){return duration;}
};

class Measure{
 private:
  int number;
  int tempo;
 public:
  Measure(int);
  ~Measure(){
    delete chords;
  }
  LinkedList<Chord>* chords;
  void print(){
    LinkedList<Chord>* t = chords;
    cout<<"Measure "<<number<<" of Tempo : "<<tempo<<endl;
    while (t != NULL){
      t->getHead()->print();
      t = t->getTail();
    }
  }
  int getNumber(){return number;}
  int getTempo(){return tempo;}
  void setTempo(int t){tempo = t;}
  
};

class Track{
 private:
  char id[100];
  char instrument_name[100];
  char part_name[100];
  
 public:
  //list of measures
  LinkedList<Measure>* measures;
  
  Track(char*);
  ~Track(){
    delete measures;
  }
  void setInstrumentName(char*);
  void setPartName(char*);
  char* getID(){return id;}
  char* getInstrumentName(){return instrument_name;}
  char* getPartName(){return part_name;}
  void print(){
    cout<<"ID : "<<id<<" | Instrument name : "<<instrument_name<<" | Part name : "<<part_name<<endl;
  }
};


class Tab{
 private:
  char work_title[100];
  char creator[100];
  
 public:
  LinkedList<Track>* tracks;
  void setWorkTitle(char*);
  void setCreator(char*);
  Tab();
  ~Tab(){
    delete tracks;
  }
};

#endif
