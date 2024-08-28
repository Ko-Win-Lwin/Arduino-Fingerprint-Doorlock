#include <Adafruit_Fingerprint.h>
#include <Arduino.h>
#include "Enrollment.h"
#include "Attendance.h"
#include "User.h"
#include "CardService.h"
#include <Wire.h>
#include "Display.h"
#include "Servo.h"

#include <Keypad.h>

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 22, 23, 24, 25 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 26, 27, 28, 29 };  //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Servo myServo;

const int relayPin = 9;  // Relay control pin

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const int chipSelect = 10;

const char* GUEST_KEY = "12345";

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup...");

  // Initialize the SD card
  CardService::begin(chipSelect);

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

  pinMode(relayPin, OUTPUT);
  myServo.attach(8);
  myServo.write(55);
  displayMenu();
}

int userInput = 0;
char* database = "user_data.csv";
char* attendance_record = "daily_record.csv";


void loop() {

  char customKey = customKeypad.getKey();

  if (customKey) {
    userInput = customKey - '0';  // Update userInput based on key pressed
    handleUserInput(userInput);
    displayMenu();
  }
}


void handleUserInput(int userInput) {
  Enrollment enrollment;
  Attendance attendance;
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
            open_door();
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
        int id = attendance.getFingerprintIDez();
        Serial.print("enroll id ");
        Serial.println(id);

        if (id == -1) {
          display.println(F("Try again."));
          delay(2000);
          return;
        }

        if (id > 0) {
          display.println(F("User already exist."));
          delay(2000);
          return;
        }

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
        String key = readStringFromKeypad(F("Enter key"));
        if (key == GUEST_KEY) {
          open_door();
        }
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
      // display.println(F("Invalid user input. You entered: "));
      display.println(userInput);
      delay(2000);
      break;
  }
}


// switch (userInput) {
//   case 1:
//     {
//       display.println(F("Attendance."));
//       int attendanceId = attendance.getFingerprintIDez();
//       Serial.print("User ID: ");
//       Serial.println(attendanceId);
//       if (attendanceId > 0) {
//         Serial.print("Found ID: ");
//         Serial.println(attendanceId);
//         User* user = CardService::readFileFromCSV(database, attendanceId);
//         if (user != nullptr) {
//           processEnrollmentData(user->getUserId(), user->getUsername(), user->getPassword(), user->getAcademic(), user->getDepartment(), user->getRollNumber());
//           CardService::saveAttendance(attendance_record, user);
//           open_door();
//         } else {
//           Serial.println("User returned null from read CSV");
//         }
//         delete user;
//       }
//       break;
//     }

//   case 2:
//     {
//       display.println(F("Enrollment."));
//       int id = attendance.getFingerprintIDez();
//       Serial.print("enroll id ");
//       Serial.println(id);

//       if (id == -1) {
//         display.println(F("Try again."));
//         delay(2000);
//         return;
//       }

//       if (id > 0) {
//         display.println(F("User already exist."));
//         delay(2000);
//         return;
//       }

//       int enrollmentId = enrollment.getFingerprint();
//       if (enrollmentId > 0) {
//         int userId = enrollmentId;
//         String username = readStringInput(F("Enter Username:"));
//         String password = readStringInput(F("Enter Password:"));
//         int academic = readIntegerInput(F("Enter Academic Year:"));
//         String department = readStringInput(F("Enter Department:"));
//         String rollNumber = readStringInput(F("Enter Roll Number:"));
//         processEnrollmentData(enrollmentId, username, password, academic, department, rollNumber);
//         User* user = new User(userId, username, password, academic, department, rollNumber);
//         CardService::writeFileToCSV(database, user);
//         delete user;
//       } else {
//         display.println(F("Enrollment failed."));
//       }
//       break;
//     }

//   case 3:
//     {
//       display.println(F("Hello Guest. Ask a temporary password from staff."));
//       String key = readStringInput(F("Enter guest key "));
//       if (key == GUEST_KEY) {
//         open_door();
//       }
//       break;
//     }

//   case 4:
//     {
//       display.clear();
//       display.println("Showing today's record.");
//       delay(1000);
//       int userCount = 0;
//       User** users = CardService::showAttendance(userCount, attendance_record);
//       for (int i = 0; i < userCount; ++i) {
//         display.print((users[i]->getUserId()));
//         display.print(".");
//         display.println(users[i]->getUsername());
//         delay(1000);
//       }
//       for (int i = 0; i < userCount; ++i) {
//         delete users[i];
//       }
//       delete[] users;
//       delay(3000);
//       break;
//     }

//   default:
//     // display.println(F("Invalid user input. You entered: "));
//     display.println(customKey);
//     delay(2000);
//     break;
// }



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

String readStringFromKeypad(const String& prompt) {
  String input = "";
  char key = '\0';

  display.println(prompt);  // Display the prompt

  while (true) {
    key = customKeypad.getKey();  // Read the keypress

    if (key) {           // If a key is pressed
      if (key == '#') {  // End input when `#` is pressed
        break;
      } else if (key == '*') {  // Handle backspace when `*` is pressed
        if (input.length() > 0) {
          input.remove(input.length() - 1);  // Remove last character
          display.print("\b \b");            // Remove last character from display
        }
      } else {
        input += key;        // Append the character to the input
        display.print(key);  // Display the character
      }
      delay(100);  // Small delay to debounce the keypad
    }
  }

  display.println();  // Newline after input
  return input;
}


void open_door() {
  delay(1000);
  digitalWrite(relayPin, HIGH);
  delay(2000);
  // TODO
  // turn servo
  myServo.write(270);
  Serial.println(F("Servo is opening "));
  delay(4000);
  myServo.write(55);
  delay(3000);
  digitalWrite(relayPin, LOW);
  delay(1000);
}