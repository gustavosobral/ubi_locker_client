/*
  Service.h - Library for consuming Web Services API's.
  Created by Gustavo Sobral, Febuary 26, 2016.
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
    String getRFAccess(String key);
    String getPWDAccess(String login, String password);
  private:
    EthernetClient _client;
    IPAddress _server;
    String handleResponse();
};

#endif
