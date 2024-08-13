#ifndef DISPLAY_H
#define DISPLAY_H

#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // OLED display address

extern SSD1306AsciiWire display;

void initDisplay();
void displayMenu();
void processEnrollmentData(int id, const String& username, const String& password, int academic, const String& department, const String& rollNumber);

#endif // DISPLAY_H
