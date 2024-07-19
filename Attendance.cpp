// Attendance.cpp
#include "Attendance.h"

// Initialize the static instance of the class
Attendance& Attendance::getInstance() {
  static Attendance instance;
  return instance;
}

// Private constructor for Singleton pattern
Attendance::Attendance() {
  // Initialization code if needed
}

// Method to get fingerprint ID
int Attendance::getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    return -1;
  }

  // found a match!
  // Serial.print("Found ID #");
  // Serial.print(finger.fingerID);
  // Serial.print(" with confidence of ");
  // Serial.println(finger.confidence);
  return static_cast<int>(finger.fingerID);
}
