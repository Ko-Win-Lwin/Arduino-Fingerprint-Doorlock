// Attendance.h
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
  // Singleton pattern method
  static Attendance& getInstance();
  
  // Method to get fingerprint ID
  int getFingerprintIDez();
  
  // Delete copy constructor and assignment operator to enforce Singleton pattern
  Attendance(const Attendance&) = delete;
  Attendance& operator=(const Attendance&) = delete;

private:
  // Private constructor for Singleton pattern
  Attendance();
};

#endif // ATTENDANCE_H
