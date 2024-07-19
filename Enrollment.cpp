#include "Enrollment.h"
#include <Adafruit_Fingerprint.h> // Include the fingerprint library

// Assuming 'finger' is declared elsewhere and initialized properly
extern Adafruit_Fingerprint finger;

// Private constructor for Singleton pattern
Enrollment::Enrollment() {
  // Initialize nextId to the current count of enrolled fingerprints + 1
  finger.getTemplateCount();
  nextId = finger.templateCount + 1;
}

// Get instance method for Singleton pattern
Enrollment& Enrollment::getInstance() {
  static Enrollment instance; // Static instance of the class
  return instance;
}

void Enrollment::getFingerprint() {
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(nextId);

  // First attempt to capture the fingerprint
  p = getFingerFirstTime();
  if (p != FINGERPRINT_OK) {
    Serial.println("First image capture failed.");
    return;
  }

  // Convert first captured image
  convertedImageT1 = convertImageT1();
  if (convertedImageT1 != FINGERPRINT_OK) {
    Serial.println("First image conversion failed.");
    return;
  }

  // Remove finger
  Serial.println("Remove finger");
  delay(2000); // Adjust delay time as needed

  // Second attempt to capture the fingerprint
  p = getFingerSecondTime();
  if (p != FINGERPRINT_OK) {
    Serial.println("Second image capture failed.");
    return;
  }

  // Convert second captured image
  convertedImageT2 = convertImageT2();
  if (convertedImageT2 != FINGERPRINT_OK) {
    Serial.println("Second image conversion failed.");
    return;
  }

  // Create fingerprint model
  p = createModel();
  if (p != FINGERPRINT_OK) {
    Serial.println("Fingerprint model creation failed.");
    return;
  }

  // Store the fingerprint model
  p = storeModel();
  if (p != FINGERPRINT_OK) {
    Serial.println("Fingerprint storage failed.");
    return;
  }

  // Increment nextId for the next enrollment
  nextId++;
  Serial.print("next id ");
  Serial.println(nextId);

  Serial.println("Enrollment successful!");
}

int Enrollment::getFingerFirstTime() {
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("First image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      default:
        Serial.println("Image capture error");
        break;
    }
    delay(1000); // Adjust delay time as needed
  }
  return p;
}

int Enrollment::convertImageT1() {
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("First image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      break;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Invalid image");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
  return p;
}

int Enrollment::getFingerSecondTime() {
  p = -1; // Reset p before second attempt
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Second image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      default:
        Serial.println("Image capture error");
        break;
    }
    delay(1000); // Adjust delay time as needed
  }
  return p;
}

int Enrollment::convertImageT2() {
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Second image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      break;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Invalid image");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
  return p;
}

int Enrollment::createModel() {
  Serial.print("Creating model for ID ");
  Serial.println(nextId);

  p = finger.createModel();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Fingerprint model created");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_ENROLLMISMATCH:
      Serial.println("Fingerprints did not match");
      break;
    default:
      Serial.println("Unknown error during model creation");
      break;
  }
  return p;
}

int Enrollment::storeModel() {
  Serial.print("Storing model for ID ");
  Serial.println(nextId);

  p = finger.storeModel(nextId);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Fingerprint stored successfully");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_BADLOCATION:
      Serial.println("Could not store in that location");
      break;
    case FINGERPRINT_FLASHERR:
      Serial.println("Error writing to flash");
      break;
    default:
      Serial.println("Unknown error during storage");
      break;
  }
  return p;
}
