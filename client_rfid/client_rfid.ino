#include <SPI.h>
#include <Keypad.h>
#include <MFRC522.h>
#include <Ethernet.h>

#include "Service.h"

// RF-ID
#define SS_PIN 9
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(162,243,249,102);
IPAddress ip(192, 168, 25, 83); // Set the static IP address to use if the DHCP fails to assign

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A0, A1, A3, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A5, 6, 7}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;

int denied = 3;
int allowed = 4;
String json = "";
String keyId = "";
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
  key = keypad.getKey();

  if (key != NO_KEY) {
    switch(key){
      case '1': requestAccessRF(); break;
      case '2': requestAccessPWD(); break;
      case '8': registerStudent(); break;
      case '9': updateRF();
    }
  }
}

String extractToken(String json) {
  json = json.substring(json.indexOf(":")+2, json.indexOf("}")-1);
  if(json.equals("Not found.")) {
    return "";
  } else {
    return json;
  }
}

void requestAccessRF() {
  Serial.println("[LOG]: requestAccessRF");
  tone(2, 2062, 6);
}

void requestAccessPWD() {
  Serial.println("[LOG]: requestAccessPWD");
  tone(2, 2062, 6);
}

void registerStudent() {
  Serial.println("[LOG]: registerStudent");
  tone(2, 2062, 6);
}

void updateRF() {
  Serial.println("[LOG]: updateRF");
  tone(2, 2062, 6);

  String login = "";
  String password = "";
  String adminKeyId = "";
  String token = "";
  keyId = "";

  while(true) {
    delay(200);

    if(token.length() == 0) {
      if(keypad.getKey() != NO_KEY)
        break;
  
      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        continue;
      }
  
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        continue;
      }
  
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        adminKeyId += mfrc522.uid.uidByte[i] < 0x10 ? "0" : "";
        adminKeyId += String(mfrc522.uid.uidByte[i], HEX);
      }
      mfrc522.PICC_HaltA(); // Stop reading

      //  json = service.getToken(adminKeyId);
      json = service.getToken("123456789");
      token = extractToken(json);
      Serial.println("[LOG]: token = " + token);
      if(token.length() != 0){
        continue;
      } else {
        digitalWrite(denied, HIGH);
        delay(1000);
        digitalWrite(denied, LOW);
        Serial.println("[LOG]: Not authorized access");
        return;
      }
    } else {
      key = keypad.getKey();
      
      if(key != NO_KEY) {
        if(key == '#')
          return;
  
        if(login.length() != 8) {
          login += key;
          tone(2, 2062, 6);
          continue;
        }
  
        if(password.length() != 4) {
          password += key;
          tone(2, 2062, 6);
          continue;
        }
      } else {
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
          continue;
        }
    
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
          continue;
        }
    
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          keyId += mfrc522.uid.uidByte[i] < 0x10 ? "0" : "";
          keyId += String(mfrc522.uid.uidByte[i], HEX);
        }
        mfrc522.PICC_HaltA(); // Stop reading
        break;
      }
    }
  }

  Serial.println("[LOG]: adminKeyId = " + adminKeyId);
  Serial.println("[LOG]: login = " + login);
  Serial.println("[LOG]: password = " + password);
  Serial.println("[LOG]: keyId = " + keyId);
  service.postRFID(token, keyId, password, login);
}
