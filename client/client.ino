#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include "Service.h"

// RF-ID
#define SS_PIN 9
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

// Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,25,42);
IPAddress ip(192, 168, 25, 150); // Set the static IP address to use if the DHCP fails to assign

int denied = 3;
int allowed = 4;
String card = "";
boolean enabled = false;

Service service;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SPI.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing...");
  mfrc522.PCD_Init();
  service.init(mac, server, ip);
  pinMode(allowed, OUTPUT);
  pinMode(denied, OUTPUT);
  Serial.println(" Ok!");
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(200);
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(200);
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card += mfrc522.uid.uidByte[i] < 0x10 ? "0" : "";
    card += String(mfrc522.uid.uidByte[i], HEX);
  }
  mfrc522.PICC_HaltA(); // Stop reading
  
  Serial.println(service.getKey(card));
  if(enabled) {
    digitalWrite(allowed, HIGH);
    delay(1000);
    digitalWrite(allowed, LOW);
  } else {
    digitalWrite(denied, HIGH);
    delay(1000);
    digitalWrite(denied, LOW);
  }
  card = "";
}

