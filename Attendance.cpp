#include "Attendance.h"

// Initialize the Adafruit_Fingerprint instance from elsewhere
extern Adafruit_Fingerprint finger;

// Constructor
Attendance::Attendance() {}

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
  return static_cast<int>(finger.fingerID);
}
