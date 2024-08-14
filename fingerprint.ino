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
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup...");

  // Initialize the SD card
  CardService::begin(chipSelect);

  Serial.println("SD card initialized.");

  Wire.begin();
  initDisplay();

  // // Initialize the fingerprint sensor
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println(F("Found fingerprint sensor!"));
    display.println(F("Found Fingerprint "));
  } else {
    Serial.println(F("Did not find fingerprint sensor :("));
    display.println(F("Fingerprint sensor not found :("));
    while (1) { delay(1); }
  }
}

int userInput = 0;
char* database = "user_data.csv";
char* attendance_record = "daily_record.csv";

void loop() {
  Enrollment enrollment;
  Attendance attendance;

  displayMenu();

  // Wait for user input and read it
  display.println(F("Waiting for input..."));
  while (Serial.available() == 0) {
    delay(10);  // Small delay to avoid CPU overload
  }

  String input = Serial.readStringUntil('\n');
  input.trim();
  userInput = input.toInt();

  // Clear any remaining data in the buffer
  while (Serial.available() > 0) {
    Serial.read();  // Read and discard each byte
  }

  switch (userInput) {
    case 1:
      {
        display.println(F("Attendance."));
        int attendanceId = attendance.getFingerprintIDez();
        Serial.print("User ID: ");
        Serial.println(attendanceId);
        if (attendanceId > 0) {
          Serial.print("Found ID: ");
          Serial.println(attendanceId);
          User* user = CardService::readFileFromCSV(database, attendanceId);
          if (user != nullptr) {
            processEnrollmentData(user->getUserId(), user->getUsername(), user->getPassword(), user->getAcademic(), user->getDepartment(), user->getRollNumber());
            CardService::saveAttendance(attendance_record, user);
            delay(3000);
          } else {
            Serial.println("User returned null from read CSV");
          }
          delete user;
        }
        break;
      }

    case 2:
      {
        display.println(F("Enrollment."));
        int enrollmentId = enrollment.getFingerprint();
        if (enrollmentId > 0) {
          int userId = enrollmentId;
          String username = readStringInput(F("Enter Username:"));
          String password = readStringInput(F("Enter Password:"));
          int academic = readIntegerInput(F("Enter Academic Year:"));
          String department = readStringInput(F("Enter Department:"));
          String rollNumber = readStringInput(F("Enter Roll Number:"));
          processEnrollmentData(enrollmentId, username, password, academic, department, rollNumber);
          User* user = new User(userId, username, password, academic, department, rollNumber);
          CardService::writeFileToCSV(database, user);
          delete user;
        } else {
          display.println(F("Enrollment failed."));
        }
        break;
      }

    case 3:
      {
        display.println(F("Hello Guest. Ask a temporary password from staff."));
        delay(2000);
        break;
      }

    case 4:
      {
        display.clear();
        display.println("Showing today's record.");
        delay(1000);
        int userCount = 0;
        User** users = CardService::showAttendance(userCount, attendance_record);
        for (int i = 0; i < userCount; ++i) {
          display.print((users[i]->getUserId()));
          display.print(".");
          display.println(users[i]->getUsername());
          delay(1000);
        }
        for (int i = 0; i < userCount; ++i) {
          delete users[i];
        }
        delete[] users;
        delay(3000);
        break;
      }

    default:
      display.print(F("Invalid user input. You entered: "));
      display.println(userInput);
      break;
  }



  // if (userInput == 1) {
  //   display.println(F("Attendance."));
  //   int id = attendance.getFingerprintIDez();
  //   Serial.print("User ID: ");
  //   Serial.println(id);
  //   if (id > 0) {
  //     Serial.print("Found ID: ");
  //     Serial.println(id);
  //     User* user = CardService::readFileFromCSV(database, id);
  //     if (user != nullptr) {
  //       processEnrollmentData(user->getUserId(), user->getUsername(), user->getPassword(), user->getAcademic(), user->getDepartment(), user->getRollNumber());
  //       CardService::saveAttendance(attendance_record, user);
  //       delay(3000);
  //     } else {
  //       Serial.println("User return null from read csv");
  //     }

  //     delete user;
  //   }
  // } else if (userInput == 2) {
  //   display.println(F("Enrollment."));
  //   int id = enrollment.getFingerprint();
  //   if (id > 0) {
  //     int userId = id;
  //     String username = readStringInput(F("Enter Username:"));
  //     String password = readStringInput(F("Enter Password:"));
  //     int academic = readIntegerInput(F("Enter Academic Year:"));
  //     String department = readStringInput(F("Enter Department:"));
  //     String rollNumber = readStringInput(F("Enter Roll Number:"));
  //     processEnrollmentData(id, username, password, academic, department, rollNumber);
  //     User* user = new User(userId, username, password, academic, department, rollNumber);
  //     CardService::writeFileToCSV(database, user);
  //     delete user;
  //   } else {
  //     display.println(F("Enrollment failed."));
  //   }
  // } else if (userInput == 3) {
  //   display.println(F("Hello Guest. Ask a temporary password from staff."));
  //   delay(2000);
  // } else if (userInput == 4) {
  //   display.clear();
  //   display.println("Showing today's record.");
  //   delay(1000);
  //   int userCount = 0;
  //   User** users = CardService::showAttendance(userCount, attendance_record);
  //   for (int i = 0; i < userCount; ++i) {
  //     display.print((users[i]->getUserId()));
  //     display.print(".");
  //     display.println(users[i]->getUsername());
  //     delay(1000);
  //   }
  //   for (int i = 0; i < userCount; ++i) {
  //     delete users[i];
  //   }
  //   delete[] users;
  //   delay(3000);
  // } else {
  //   display.print(F("Invalid user input. You entered: "));
  //   display.println(userInput);
  // }
}

int readIntegerInput(const String& prompt) {
  display.println(prompt);
  while (Serial.available() == 0) {
    delay(10);  // Small delay to avoid CPU overload
  }
  int value = Serial.parseInt();
  Serial.read();  // Clear the newline character
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
  while (Serial.available() > 0) {
    Serial.read();  // Read and discard each byte
  }
  return input;
}
