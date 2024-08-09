#include <SdFat.h>
#include "CardService.h"
#include "user.h"

SdFat CardService::sd;

void CardService::begin(int chipSelectPin) {
  if (!sd.begin(chipSelectPin)) {
    Serial.println("sd card initialization failed!");
  } else {
    Serial.println("sd card initialized.");
  }
}


// write to csv
void CardService::writeJsonToCSV(const char* filename, User* user) {
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
User* CardService::readJsonFromCSV(const char* filename, int fingerId) {
  File file = sd.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return nullptr;;
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