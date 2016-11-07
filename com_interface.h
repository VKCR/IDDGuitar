#ifndef COM_INTERFACE_H
#define COM_INTERFACE_H

#include <iostream>
#include "Tab.h"
using namespace std;

class com_interface{
 public:
  void Track2String(Track*, char*);
  void Measure2String(Measure*, char*);
  
  com_interface(){};
  virtual void Stream(Track* t, int m1, int m2){cout<<"not yet implemented"<<endl;}
  virtual void Stop(){cout<<"not yet implemented"<<endl;}
  virtual void Start(){cout<<"not yet implemented"<<endl;}
  virtual void ReadInfinity(){cout<<"not yet implemented"<<endl;}
};

#endif
