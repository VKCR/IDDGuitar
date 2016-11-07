#ifndef COM_INTERFACE_SERIAL_H
#define COM_INTERFACE_SERIAL_H

#include <fcntl.h>
#include <unistd.h>
#include "com_interface.h"
#include "Tab.h"



class com_interface_serial : public com_interface{
 private:
  int serial_controller;
  void publish(const char*);
  bool ACKed = false;
  bool OKed = false;
  int lastOKed = 0;
  void RecvMSG();
  void DispatchMSG(char*);
  enum MSG {ACK, OK};
 public:
  com_interface_serial();
  com_interface_serial(const char*);
  void Stream(Track*, int, int);
  void Stop();
  void Start();
  void ReadInfinity();
};

#endif
