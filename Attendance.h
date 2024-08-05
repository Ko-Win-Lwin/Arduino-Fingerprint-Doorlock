#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include "HardwareSerial.h"
#include "Adafruit_Fingerprint.h"

// Ensure that the Adafruit_Fingerprint instance is defined elsewhere
extern Adafruit_Fingerprint finger;

class Attendance {
private:
  int p = -1;  // Initialize p to -1
  int convertedImageT1;

public:
  // Constructor
  Attendance();

  // Method to get fingerprint ID
  int getFingerprintIDez();

  // Copy constructor and assignment operator are default (public)
  // No need to delete them as we are not enforcing Singleton pattern
};

#endif // ATTENDANCE_H
