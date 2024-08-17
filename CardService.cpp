#include "Arduino.h"
#include "HardwareSerial.h"
#include "common/ArduinoFiles.h"
#include <SdFat.h>
#include "CardService.h"
#include "User.h"
#include "Array.h"
#include "Display.h"

const int ELEMENT_COUNT_MAX = 10;  // Adjust size as needed

// Create an Array instance for User pointers
Array<User*, ELEMENT_COUNT_MAX> users;

SdFat CardService::sd;

void CardService::begin(int chipSelectPin) {
  if (!sd.begin(chipSelectPin)) {
    Serial.println("sd card initialization failed!");
  } else {
    Serial.println("sd card initialized.");
    display.println("Card initialized");
  }
}

void CardService::saveAttendance(const char* filename, User* user) {
  // Check if the file exists before trying to open it
  if (!sd.exists(filename)) {
    Serial.print("File does not exist: ");
    Serial.println(filename);
    return;
  }

  File file = sd.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  // Check if the user already exists
  String userIdStr = String(user->getUserId()); // Convert user ID to String
  bool userExists = false;

  while (file.available()) {
    String line = file.readStringUntil('\n');
    // Check if the line starts with the user ID followed by a comma
    if (line.startsWith(userIdStr + ",")) {
      Serial.print("user is ");Serial.println(userIdStr);
      userExists = true;
      break;
    }
  }

  file.close();

  if (userExists) {
    display.clear();
    display.println("Already done.");
    display.println("ma nauk nak kwar :) ");
    delay(3000);
    return;
  }

  // Open the file for writing, to append data if user does not exist
  file = sd.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // Write user data to the file, separating fields with commas
  file.print(user->getUserId());
  file.print(",");
  file.print(user->getUsername());
  file.print(",");
  file.print(user->getPassword());
  file.print(",");
  file.print(user->getAcademic());
  file.print(",");
  file.print(user->getDepartment());
  file.print(",");
  file.println(user->getRollNumber());

  file.close();
  Serial.println(F("Attendance saved."));
}



User** CardService::showAttendance(int& userCount, char* filename) {
  Serial.println(filename);
  // Reset the Array instance
  users.clear();
  // Check if the file exists before trying to open it
  if (!sd.exists(filename)) {
    Serial.print("File does not exist: ");
    Serial.println(filename);
    userCount = 0;
    return nullptr;
  }

  File file = sd.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    userCount = 0;
    return nullptr;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    int index = 0;
    String fields[6];
    String field;

    for (char c : line) {
      if (c == ',') {
        fields[index++] = field;
        field = "";
      } else {
        field += c;
      }
    }
    fields[index] = field;

    if (index == 5) {                    // Check if exactly 6 fields were parsed
      int userId = fields[0].toInt();    // Convert to int
      int academic = fields[3].toInt();  // Convert to int

      // Check if Array is full
      if (users.size() >= ELEMENT_COUNT_MAX) {
        Serial.println("Array limit reached.");
        break;
      }

      users.push_back(new User(userId, fields[1], fields[2], academic, fields[4], fields[5]));
    }
  }
  file.close();

  // Convert Array to User** and set userCount
  User** finalUsers = new User*[users.size()];
  for (int i = 0; i < users.size(); ++i) {
    finalUsers[i] = users[i];
  }

  userCount = users.size();
  return finalUsers;
}

// write to csv
void CardService::writeFileToCSV(const char* filename, User* user) {
  File file = sd.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // Write user data to the file, separating fields with commas
  file.print(user->getUserId());
  file.print(",");
  file.print(user->getUsername());
  file.print(",");
  file.print(user->getPassword());
  file.print(",");
  file.print(user->getAcademic());
  file.print(",");
  file.print(user->getDepartment());
  file.print(",");
  file.println(user->getRollNumber());

  file.close();
  Serial.println(F("User data written to CSV file"));
  return;
}



// read from csv
User* CardService::readFileFromCSV(const char* filename, int fingerId) {
  File file = sd.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("Reading user data from CSV file:");

  while (file.available()) {
    String line = file.readStringUntil('\n');
    int index = 0;
    String values[6];  // Adjust size based on number of fields

    // Split line by commas
    while (line.length() > 0) {
      int commaIndex = line.indexOf(',');
      if (commaIndex == -1) {
        values[index++] = line;
        break;
      }
      values[index++] = line.substring(0, commaIndex);
      line = line.substring(commaIndex + 1);
    }


    if (values[0].toInt() == fingerId) {
      User* user = new User(
        values[0].toInt(),
        values[1],
        values[2],
        values[3].toInt(),
        values[4],
        values[5]);
      file.close();
      return user;
    }
  }
  file.close();
  return nullptr;
}




// delete csv
void CardService::deleteFileFromCSV(const char* filename) {
  if (sd.exists(filename)) {
    if (sd.remove(filename)) {
      Serial.println("File deleted successfully.");
    } else {
      Serial.println("Failed to delete the file.");
    }
  } else {
    Serial.println("File does not exit.");
  }
}