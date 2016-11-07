#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "com_interface_serial.h"


using namespace std;

//initialize the serial connection 
com_interface_serial::com_interface_serial(){  
  serial_controller = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  if (serial_controller == -1){
    /* Could not open the port. */
    cout<<"open_port: Unable to open /dev/ttyACM0 - "<<endl;
    exit(0);
  }
}

com_interface_serial::com_interface_serial(const char* serial_port){  
  serial_controller = open(serial_port, O_RDWR | O_NOCTTY);
  if (serial_controller == -1){
    /* Could not open the port. */
    cout<<"open_port: Unable to open "<<serial_port<<endl;
    exit(0);
  }
}

//push a text on the serial controller
//IMPROVEMENTS COULD BE DONE ON THE ACK SYSTEM
void com_interface_serial::publish(const char* s){

  char ACKbuff[100];
  
  if(strlen(s)<10000){
    write(serial_controller, s, strlen(s)*sizeof(char));
    read(serial_controller, ACKbuff, sizeof(ACKbuff));
    cout<<ACKbuff<<endl;
  }
  else{ //we have to segment the array
    int size = strlen(s);
    int index = 0;
    char sendbuf[10001];
    while (size != 0){ //check that this works lol
      memcpy(sendbuf, &s[index*10000], (10000 < size ? 10000 : size));
      10000 < size ? sendbuf[10000] = 0 : sendbuf[size] = 0;
      
      write(serial_controller, s, strlen(sendbuf)*sizeof(char));

      10000 < size ? size-=10000 : size=0;

      read(serial_controller, ACKbuff, sizeof(ACKbuff));
      cout<<ACKbuff<<endl;
    }
    
  }
}


//stream measures to play
void com_interface_serial::Stream(Track* t, int m1, int m2){
  char* measurebuf;
  char ACKbuff[100];
  LinkedList<Measure>* mlist = t->measures;
  int sent = 0;
  int played = 0;
  
  //go to the start of the measure
  while(mlist->getHead()->getNumber() != m1)
    mlist = mlist->getTail();

  //now for each measure until the end measure is reached
  while(mlist->getHead()->getNumber() != m2+1){
    if (sent-played < 5){
      cout<<"Publishing measure : "<<mlist->getHead()->getNumber()<<endl;
      Measure2String(mlist->getHead(), &measurebuf);
      publish(measurebuf);
      sent++;

      free(measurebuf); //don't forget to free the memory
      mlist = mlist->getTail(); //go to the next measure
    }
    else{
      read(serial_controller, ACKbuff,sizeof(ACKbuff));
      if (!strcmp(ACKbuff, "OK"))
	played++;
    }
  }
}

//transfer the order to stop
void com_interface_serial::Stop(){
  publish("{STOP}");
}

