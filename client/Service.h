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
    String getToken(String key);
    String postStudent(String token, String rfid, String password, String login);
    String postRFID(String token, String rfid, String password, String login);
  private:
    EthernetClient _client;
    IPAddress _server;
    String handleResponse();
};

#endif
