#include <Adafruit_Fingerprint.h>
#include <Arduino.h>
#include "Enrollment.h"
#include "Attendance.h"
#include "User.h"
#include "CardService.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // OLED display address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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


  // if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for (;;);  // Don't proceed, loop forever
  // }

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println(F("Found fingerprint sensor!"));
  } else {
    Serial.println(F("Did not find fingerprint sensor :("));
    while (1) { delay(1); }
  }
}




int userInput = 0;


void loop() {
  Enrollment enrollment;
  Attendance attendance;

  display_menu();

  // Wait for user input and read it
  Serial.println(F("Waiting for input..."));
  while (Serial.available() == 0) {
    delay(10);  // Small delay to avoid CPU overload
  }

  // Read the input from Serial
  String input = Serial.readStringUntil('\n');
  input.trim();

  // Debugging: Print the raw input to see what was read
  // Serial.print(F("Raw input received: '"));
  // Serial.print(input);
  // Serial.println(F("'"));

  // Convert the input to integer
  userInput = input.toInt();

  // Debugging: Print the converted integer
  Serial.print(F("User Input (converted to int): "));
  Serial.println(userInput);

  // Clear any remaining data in the buffer
  while (Serial.available() > 0) {
    Serial.read();  // Read and discard each byte
  }

  if (userInput == 1) {
    Serial.println(F("Attendance."));
    int id = attendance.getFingerprintIDez();
    if (id > 0) {
      Serial.print("found id "); Serial.println(id);
      // TODO 
      // get user with this id from database
      // open lock
    }
    // Process the user input
  } else if (userInput == 2) {
    Serial.println(F("Enrollment."));
    int id = enrollment.getFingerprint();
    if (id > 0) {
      int userId = id;
      String username = readStringInput(F("Enter Username:"));
      String password = readStringInput(F("Enter Password:"));
      int academic = readIntegerInput(F("Enter Academic Year:"));
      String department = readStringInput(F("Enter Department:"));
      String rollNumber = readStringInput(F("Enter Roll Number:"));
      processEnrollmentData(id, username, password, academic, department, rollNumber);
      // TODO
      // save to database
    } else {
      Serial.println(F("Enrollment failed."));
    }
  } else {
    Serial.print(F("Invalid user input. You entered: "));
    Serial.println(userInput);
  }
}





void display_menu() {
  Serial.println(F("Main Menu"));
  Serial.println(F("1. Attendance."));
  Serial.println(F("2. Enrollment."));
}


// Function to read an integer input
int readIntegerInput(const String& prompt) {
  Serial.println(prompt);
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


// Function to read a string input
String readStringInput(const String& prompt) {
  Serial.println(prompt);
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

// Function to process the collected enrollment data
// Function to process the collected enrollment data
void processEnrollmentData(int id, const String& username, const String& password, int academic, const String& department, const String& rollNumber) {
  Serial.print(F("ID: "));
  Serial.println(id);
  Serial.print(F("Username: "));
  Serial.println(username);
  Serial.print(F("Password: "));
  Serial.println(password);
  Serial.print(F("Academic Year: "));
  Serial.println(academic);
  Serial.print(F("Department: "));
  Serial.println(department);
  Serial.print(F("Roll Number: "));
  Serial.println(rollNumber);
}