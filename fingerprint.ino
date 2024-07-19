#include <Adafruit_Fingerprint.h>
#include <Arduino.h>

#include "Enrollment.h"
#include "Attendance.h"
#include "User.h"


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


void setup() {
  Serial.begin(9600);
  mySerial.begin(57600);  // Initialize the fingerprint sensor serial communication


  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1000); }
  }
  Serial.print("get template count");
  Serial.println(finger.getTemplateCount());
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);


  User *user1 = new User(1, "win lwin", "password", 5, "MC", "ext 3");
  User *user2 = new User(2, "zin bo khine", "password", 5, "MC", "ext 5");
  User *user3 =  new User(3, "may su lei yee htay", "password", 5, "MC", "1");
  User *user4 =  new User(4, "nilar tun", "password", 5, "MC", "ext 1");
  User *user5 =  new User(5, "kyi khant", "password", 5, "MC", "ext 4");
  User::addUser(user1);
  User::addUser(user2);
  User::addUser(user3);
  User::addUser(user4);
  User::addUser(user5);
  
}

void loop() {
  // Enrollment &enrollment = Enrollment::getInstance();
  // enrollment.getFingerprint();
  // delay(5000); // Delay between enrollment attempts

  // Attendance &attendance = Attendance::getInstance();
  // int id = attendance.getFingerprintIDez();
  // if(id > 0) {
  //   User::findAndPrintUser(id);
  // }

  delay(3000);
}
