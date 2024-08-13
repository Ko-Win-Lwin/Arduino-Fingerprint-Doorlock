#include <Adafruit_Fingerprint.h>
#include <Arduino.h>
#include "Enrollment.h"
#include "Attendance.h"
#include "User.h"
#include "CardService.h"
#include <Wire.h>
#include "Display.h"



#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);
#else
#define mySerial Serial
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  CardService::begin(10);  // Use the correct chip select pin

  Wire.begin();
  initDisplay();

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println(F("Found fingerprint sensor!"));
    display.println(F("Fingerprint sensor found!"));
  } else {
    Serial.println(F("Did not find fingerprint sensor :("));
    display.println(F("Fingerprint sensor not found :("));
    while (1) { delay(1); }
  }
}

int userInput = 0;

void loop() {
  Enrollment enrollment;
  Attendance attendance;

  displayMenu();

  // Wait for user input and read it
  display.println(F("Waiting for input..."));
  while (Serial.available() == 0) {
    delay(10);  // Small delay to avoid CPU overload
  }

  // Read the input from Serial
  String input = Serial.readStringUntil('\n');
  input.trim();

  // Convert the input to integer
  userInput = input.toInt();
  // Clear any remaining data in the buffer
  while (Serial.available() > 0) {
    Serial.read();  // Read and discard each byte
  }

  if (userInput == 1) {
    display.println(F("Attendance."));
    int id = attendance.getFingerprintIDez();
    Serial.print("user id ");
    Serial.println(id);
    if (id > 0) {
      Serial.print("found id ");
      Serial.println(id);
      User* user = CardService::readJsonFromCSV("user_data.csv", id);
      display.clear();
      processEnrollmentData(user->getUserId(), user->getUsername(), user->getPassword(), user->getAcademic(), user->getDepartment(), user->getRollNumber());
      delay(3000);
      delete user;
      id = 0;
    }
  } else if (userInput == 2) {
    display.println(F("Enrollment."));
    int id = enrollment.getFingerprint();
    if (id > 0) {
      int userId = id;
      String username = readStringInput(F("Enter Username:"));
      String password = readStringInput(F("Enter Password:"));
      int academic = readIntegerInput(F("Enter Academic Year:"));
      String department = readStringInput(F("Enter Department:"));
      String rollNumber = readStringInput(F("Enter Roll Number:"));
      processEnrollmentData(id, username, password, academic, department, rollNumber);
      User* user = new User(userId, username, password, academic, department, rollNumber);
      CardService::writeJsonToCSV("user_data.csv", user);
      delete user;
      id = 0;
    } else {
      display.println(F("Enrollment failed."));
    }
  } else {
    display.print(F("Invalid user input. You entered: "));
    display.println(userInput);
  }
}



int readIntegerInput(const String& prompt) {
  display.println(prompt);
  while (Serial.available() == 0) {
    delay(10);  // Small delay to avoid CPU overload
  }

  int value = Serial.parseInt();
  Serial.read();  // Clear the newline character

  // Clear any remaining data in the buffer
  while (Serial.available() > 0) {
    Serial.read();  // Read and discard each byte
  }

  return value;
}

String readStringInput(const String& prompt) {
  display.println(prompt);
  while (Serial.available() == 0) {
    delay(10);  // Small delay to avoid CPU overload
  }

  String input = Serial.readStringUntil('\n');
  input.trim();  // Remove any leading or trailing whitespace

  // Clear any remaining data in the buffer
  while (Serial.available() > 0) {
    Serial.read();  // Read and discard each byte
  }

  return input;
}
