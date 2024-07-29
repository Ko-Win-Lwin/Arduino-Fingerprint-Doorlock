#ifndef CARDSERVICE_H
#define CARDSERVICE_H

#include <SdFat.h>
#include <ArduinoJson.h>
#include "User.h"

class CardService {
public:
    static void begin(int chipSelectPin);
    static void writeJsonToSD(const char* filename, User *user);
    static void readJsonFromSD(const char* filename);
    static void deleteFileFromSD(const char* filename);

private:
    static SdFat sd;
};

#endif // CARDSERVICE_H
