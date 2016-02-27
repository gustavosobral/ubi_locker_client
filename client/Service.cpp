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

String Service::getKey(String key)
{
  boolean stringComplete = false;
  String inputString = "";

  // if you get a connection, report back via serial:
  if (_client.connect(_server, 8000)) {
    // Make a HTTP request:
    _client.println("GET /api/keys/" + key + " HTTP/1.1");
    _client.println("Host: 192.168.25.42:8000");
    _client.println("User-Agent: arduino-ethernet");
    _client.println("Connection: close");
    _client.println();
  } else {
    // if you didn't get a connection to the server:
     Serial.println("connection failed");
  }
  
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

  return inputString;
}

