#include "Display.h"
#include <Arduino.h>

// Initialize the global display object
SSD1306AsciiWire display;

void initDisplay() {
  display.begin(&Adafruit128x64, SCREEN_ADDRESS);
  display.setFont(Adafruit5x7);

  // Clear the screen
  display.clear();
  display.println(F("SSD1306Ascii Initialized"));
}

void displayMenu() {
  display.clear();
  display.println(F("Main Menu"));
  display.println(F("1. Attendance."));
  display.println(F("2. Enrollment."));
  display.println();
  Serial.println(F("Main Menu"));
  Serial.println(F("1. Attendance."));
  Serial.println(F("2. Enrollment."));
}


void processEnrollmentData(int id, const String& username, const String& password, int academic, const String& department, const String& rollNumber) {
  display.print(F("ID: "));
  display.println(id);
  display.print(F("Username: "));
  display.println(username);
  display.print(F("Password: "));
  display.println(password);
  display.print(F("Academic Year: "));
  display.println(academic);
  display.print(F("Department: "));
  display.println(department);
  display.print(F("Roll Number: "));
  display.println(rollNumber);
}
