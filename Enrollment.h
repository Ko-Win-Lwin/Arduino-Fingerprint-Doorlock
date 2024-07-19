#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <Arduino.h>
#include <Adafruit_Fingerprint.h> // Assuming you're using this library for fingerprint

class Enrollment {
private:
  int p = -1; // Initialize p to -1
  int convertedImageT1;
  int convertedImageT2;
  int nextId; // Initialize nextId to 1 for the first fingerprint

  Enrollment(); // Private constructor for Singleton pattern

  int getFingerFirstTime();
  int convertImageT1();
  int getFingerSecondTime();
  int convertImageT2();
  int createModel();
  int storeModel();

public:
  // Get instance method for Singleton pattern
  static Enrollment& getInstance();

  void getFingerprint();
};

#endif // ENROLLMENT_H
