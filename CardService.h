#ifndef CARDSERVICE_H
#define CARDSERVICE_H

#include <SdFat.h>
#include "User.h"

class CardService {
public:
    static void begin(int chipSelectPin);
    static void writeJsonToCSV(const char* filename, User *user);
    static User* readJsonFromCSV(const char* filename, int fingerId);
    static void deleteFileFromCSV(const char* filename);

private:
    static SdFat sd;
};

#endif // CARDSERVICE_H
