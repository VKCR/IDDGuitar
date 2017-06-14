#include <stdio.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include "TabReaderXML.h"
#include "com_interface_serial.h"

using namespace std;




int main(int argc, char** argv){
  Tab tab;
  char filename[100];
  if (argc > 1)
    strcpy(filename, argv[1]);
  else{
    strcpy(filename, "eagles.xml");
  }

  TabReaderXML::ReadTabXML(filename,&tab);
  tab.tracks->print();
  tab.tracks->get("P2")->measures->print();

  com_interface* COM;
  char buff[100000];
  COM = new com_interface();
  COM->Track2String(tab.tracks->get("P2"), buff);
  cout<<buff<<endl;
  delete COM;
  
  /*COM = new com_interface_serial();
  int m1 = 1;
  int m2 = 5;
  if (argc > 2){
    m1 = atoi(argv[2]);
    m2 = atoi(argv[3]);
  }

  COM->Stream(tab.tracks->get("P2"), 1, 5);
  //COM->Stream(tab.tracks->get("P7"), 105, 109);
  COM->Start();
  //COM->Stop();
  COM->ReadInfinity();*/
  return 0;
}

