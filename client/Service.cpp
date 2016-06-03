/*
  Service.cpp - Library for consuming Web Services API's.
  Created by Gustavo Sobral, Febuary 26, 2016.
*/

#include "Arduino.h"
#include "Service.h"

Service::Service(){};

void Service::init(byte * mac, IPAddress server, IPAddress ip)
{
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac, ip);
  }
  _server = server;
}

String Service::getRFAccess(String key)
{
  // if you get a connection, report back via serial:
  if (_client.connect(_server, 80)) {
    // Make a HTTP request:
    _client.println("GET /api/access/get-by-rfid/?rfid=" + key + "&locker_id=1 HTTP/1.1");
    _client.println("Host: 162.243.249.102");
    _client.println("User-Agent: arduino-ethernet");
    _client.println("Accept: application/json");
    _client.println("Content-Type: application/json");
    _client.println("Connection: close");
    _client.println();
  } else {
    // if you didn't get a connection to the server:
     Serial.println("connection failed");
     return "";
  }
  return handleResponse();
}

String Service::getPWDAccess(String login, String password) 
{
  // if you get a connection, report back via serial:
  if (_client.connect(_server, 80)) {
    // Make a HTTP request:
    _client.println("GET /api/access/get-by-pass/?locker_pass=" + password + "&amp;matriculation=" + login + "&amp;locker_id=1 HTTP/1.1");
    _client.println("Host: 162.243.249.102");
    _client.println("User-Agent: arduino-ethernet");
    _client.println("Accept: application/json");
    _client.println("Content-Type: application/json");
    _client.println("Connection: close");
    _client.println();
  } else {
    // if you didn't get a connection to the server:
     Serial.println("connection failed");
     return "";
  }
  return handleResponse();
}

String Service::handleResponse() {
  boolean stringComplete = false;
  String inputString = "";
  
  while(true) {
    if (_client.available()) {
      char c = _client.read();
      inputString += c;

      // Skips the HTTP Header
      if(inputString == "\r\n") {
        inputString = "";
        stringComplete = true;
      }
      // Clear after each new line
      if(!stringComplete && c == '\n') {
        inputString = "";
      }
    }

      // if the server's disconnected, stop the client:
    if (!_client.connected()) {
      _client.stop();
      break;
    }
  }
  
  inputString = inputString.substring(inputString.indexOf("{"), inputString.lastIndexOf("}")+1);
  return inputString;
}

