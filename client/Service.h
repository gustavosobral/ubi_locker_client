/*
  Service.h - Library for consuming API.
  Created by Gustavo Sobral, Febuary 26, 2016.
  Released into the public domain.
*/

#ifndef Service_h
#define Service_h

#include "Arduino.h"

#include <SPI.h>
#include <Ethernet.h>

class Service
{
  public:
    Service();
    void init(byte * mac, IPAddress server, IPAddress ip);
    String getKey();
  private:
    EthernetClient client;
    IPAddress _server;
};

#endif