#include "Enrollment.h"
#include <Adafruit_Fingerprint.h>  // Include the fingerprint library

// Assuming 'finger' is declared and initialized elsewhere
extern Adafruit_Fingerprint finger;

// Constructor
Enrollment::Enrollment() {
  // Initialize nextId to the current count of enrolled fingerprints + 1
  if (finger.getTemplateCount() == FINGERPRINT_OK) {
    nextId = finger.templateCount + 1;
  } else {
    Serial.println(F("Failed to get fingerprint template count."));
    nextId = 1;  // Default to 1 if initialization fails
  }
}

int Enrollment::getFingerprint() {
  int id = 0;
  Serial.print(F("Waiting for valid finger to enroll as #"));
  Serial.println(nextId);

  // First attempt to capture the fingerprint
  p = getFingerFirstTime();
  if (p != FINGERPRINT_OK) {
    Serial.println(F("First image capture failed."));
    return 0;
  }

  // Convert first captured image
  convertedImageT1 = convertImageT1();
  if (convertedImageT1 != FINGERPRINT_OK) {
    Serial.println(F("First image conversion failed."));
    return 0;
  }

  // Remove finger
  Serial.println(F("Remove finger"));
  delay(2000); // Adjust delay time as needed

  // Second attempt to capture the fingerprint
  p = getFingerSecondTime();
  if (p != FINGERPRINT_OK) {
    Serial.println(F("Second image capture failed."));
    return 0;
  }

  // Convert second captured image
  convertedImageT2 = convertImageT2();
  if (convertedImageT2 != FINGERPRINT_OK) {
    Serial.println(F("Second image conversion failed."));
    return 0;
  }

  // Create fingerprint model
  p = createModel();
  if (p != FINGERPRINT_OK) {
    Serial.println(F("Fingerprint model creation failed."));
    return 0;
  }

  // Store the fingerprint model
  p = storeModel();
  if (p != FINGERPRINT_OK) {
    Serial.println(F("Fingerprint storage failed."));
    return 0;
  }

  // Increment nextId for the next enrollment
  id = nextId;
  nextId++;
  Serial.print(F("Next ID: "));
  Serial.println(nextId);

  Serial.println(F("Enrollment successful!"));
  return static_cast<int>(id);
}

int Enrollment::getFingerFirstTime() {
  p = -1;  // Initialize p to an invalid value
  Serial.println(F("Place your finger on the sensor."));
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK) {
      Serial.println(F("First image taken."));
    } else {
      Serial.println(F("Error capturing first image."));
    }
    delay(1000);  // Adjust delay time as needed
  }
  return p;
}

int Enrollment::convertImageT1() {
  p = finger.image2Tz(1);
  if (p == FINGERPRINT_OK) {
    Serial.println(F("First image converted."));
  } else {
    Serial.println(F("Error converting first image."));
  }
  return p;
}

int Enrollment::getFingerSecondTime() {
  p = -1;  // Reset p before second attempt
  Serial.println(F("Place the same finger again."));
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK) {
      Serial.println(F("Second image taken."));
    } else {
      Serial.println(F("Error capturing second image."));
    }
    delay(1000);  // Adjust delay time as needed
  }
  return p;
}

int Enrollment::convertImageT2() {
  p = finger.image2Tz(2);
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Second image converted."));
  } else {
    Serial.println(F("Error converting second image."));
  }
  return p;
}

int Enrollment::createModel() {
  Serial.print(F("Creating model for ID "));
  Serial.println(nextId);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Fingerprint model created."));
  } else {
    Serial.println(F("Error creating model."));
  }
  return p;
}

int Enrollment::storeModel() {
  Serial.print(F("Storing model for ID "));
  Serial.println(nextId);

  p = finger.storeModel(nextId);
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Fingerprint stored successfully."));
  } else {
    Serial.println(F("Error storing fingerprint."));
  }
  return p;
}
