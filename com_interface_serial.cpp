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
  cout<<"publishing : "<<s<<endl;
  if(strlen(s)<10000){
    write(serial_controller, s, strlen(s)*sizeof(char));
    while(ACKed == false)
      RecvMSG();
    cout<<"ACked"<<endl;
    ACKed = false;
  }
  else{ //we have to segment the array
    int size = strlen(s);
    int index = 0;
    char sendbuf[10001];
    while (size != 0){ 
      memcpy(sendbuf, &s[index*10000], (10000 < size ? 10000 : size));
      10000 < size ? sendbuf[10000] = 0 : sendbuf[size] = 0;
      
      write(serial_controller, s, strlen(sendbuf)*sizeof(char));

      10000 < size ? size-=10000 : size=0;

      while(ACKed == false)
	RecvMSG();
      cout<<"ACked"<<endl;
      ACKed = false;
    }
    
  }
}

void com_interface_serial::RecvMSG(){
  cout<<"reading"<<endl;
  char ACKbuff[100];
  read(serial_controller, ACKbuff, sizeof(ACKbuff));
  char *token;
  char s[2] = "\n";

  token = strtok(ACKbuff, s);
  while( token != NULL ){
    DispatchMSG(token);
    token = strtok(NULL, s);
  }
}

void com_interface_serial::ReadInfinity(){
  cout<<"reading"<<endl;
  while(1)
    RecvMSG();
}

void com_interface_serial::DispatchMSG(char* msg){
  int m = -1;
  cout<<"Received msg : "<<msg<<endl;
  
  if (msg[0] == 'A' && msg[1] == 'C' && msg[2] == 'K'){//!strcmp(msg, "ACK")){
    m = ACK;
  }
  else if(msg[0] == 'O' && msg[1] == 'K'){
    m = OK;
  }
  
  switch(m){
  case ACK:
    ACKed = true;
    break;
  case OK:
    OKed = true;
    int l = strlen(msg) - 2;
    memcpy(msg, &msg[2], l);
    msg[l] = 0;
    lastOKed = atoi(msg);
    break;
  }
}


//stream measures to play
void com_interface_serial::Stream(Track* t, int m1, int m2){
  char measurebuf[1000]; //might have to make it bigger
  char ACKbuff[100];
  LinkedList<Measure>* mlist = t->measures;
  
  //go to the start of the measure
  while(mlist->getHead()->getNumber() != m1)
    mlist = mlist->getTail();

  int sent = mlist->getHead()->getNumber();
  lastOKed = mlist->getHead()->getNumber();
  //cout<<"publishing size"<<endl;
  publish("SIZE");
  sprintf(measurebuf, "%d", m2-m1+1); 
  publish(measurebuf);
  //cout<<"published size"<<endl;
  //now for each measure until the end measure is reached
  while(mlist->getHead()->getNumber() != m2+1){
    if (1){//sent-lastOKed < 5){ //remove this
      //cout<<"Publishing measure : "<<mlist->getHead()->getNumber()<<endl;
      Measure2String(mlist->getHead(), measurebuf);
      publish(measurebuf);
      sent++;
      mlist = mlist->getTail(); //go to the next measure
    }
    else{
      while(OKed == false)
	RecvMSG();
      cout<<"OKed number "<<lastOKed<<endl;
      OKed = false; 
    }
  }
  
}

//transfer the order to stop
void com_interface_serial::Stop(){
  publish("STOP");
}

void com_interface_serial::Start(){
  publish("START");
}
