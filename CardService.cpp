#include "WString.h"
#include "CardService.h"
#include "User.h"

SdFat CardService::sd;

void CardService::begin(int chipSelectPin) {
  if (!sd.begin(chipSelectPin, SD_SCK_MHZ(50))) {
    Serial.println("SD card initialization failed!");
  } else {
    Serial.println("SD card initialized.");
  }
}

void CardService::writeJsonToSD(const char* filename, User* user) {
  File file = sd.open(filename, FILE_WRITE);
  if (file) {
    // Create a dynamic JSON document
    DynamicJsonDocument user_record(1024);  // Adjust size as needed
    user_record["id"] = user->getUserId();
    user_record["name"] = user->getUsername();
    user_record["password"] = user->getPassword();
    user_record["academic"] = user->getAcademic();
    user_record["department"] = user->getDepartment();
    user_record["roll_number"] = user->getRollNumber();

    // Serialize JSON to file
    if (serializeJson(user_record, file) == 0) {
      Serial.println("Failed to write JSON to file.");
    } else {
      Serial.println("JSON written to file.");
    }
    file.close();
  } else {
    Serial.println("Failed to open file for writing.");
  }
}

void CardService::readJsonFromSD(const char* filename) {
  File file = sd.open(filename);
  if (file) {
    // Create a dynamic JSON document
    DynamicJsonDocument user_record(1024);  // Adjust size as needed

    DeserializationError error = deserializeJson(user_record, file);
    if (!error) {
      int id = user_record["id"];
      String name = user_record["name"].as<String>();
      String password = user_record["password"].as<String>();
      int academic = user_record["academic"];
      String department = user_record["department"].as<String>();
      String roll_number = user_record["roll_number"].as<String>();

      Serial.print("ID: ");
      Serial.println(id);
      Serial.print("Name: ");
      Serial.println(name);
      Serial.print("Password: ");
      Serial.println(password);
      Serial.print("Academic: ");
      Serial.println(academic);
      Serial.print("Department: ");
      Serial.println(department);
      Serial.print("Roll Number: ");
      Serial.println(roll_number);

    } else {
      Serial.println("Failed to parse JSON.");
    }
    file.close();
  } else {
    Serial.println("Failed to open file for reading.");
  }
}

void CardService::deleteFileFromSD(const char* filename) {
  if (sd.exists(filename)) {
    if (sd.remove(filename)) {
      Serial.println("File deleted.");
    } else {
      Serial.println("Failed to delete file.");
    }
  } else {
    Serial.println("File does not exist.");
  }
}
