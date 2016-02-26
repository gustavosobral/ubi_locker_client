#include <SPI.h>
#include <Ethernet.h>
#include "Service.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192,168,25,42);

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 25, 150);

Service service;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

 service.init(mac, server, ip);
}

void loop() {
  Serial.println(service.getKey());
  delay(3000);
}

