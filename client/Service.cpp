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

String Service::getKey()
{
  boolean stringComplete = false;
  String inputString = "";

  // if you get a connection, report back via serial:
  if (client.connect(_server, 8000)) {
    // Make a HTTP request:
    client.println("GET /api/keys HTTP/1.1");
    client.println("Host: 192.168.25.42:8000");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
     Serial.println("connection failed");
  }
  
  while(true) {
    if (client.available()) {
      char c = client.read();
      inputString += c;
  
      if(inputString == "\r\n") {
        inputString = "";
        stringComplete = true;
      }
      if(!stringComplete && c == '\n') {
        inputString = "";
      }
    }

      // if the server's disconnected, stop the client:
    if (!client.connected()) {
      client.stop();
      break;
    }
  }

  return inputString;
}

