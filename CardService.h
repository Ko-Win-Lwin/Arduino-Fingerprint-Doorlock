#ifndef CARDSERVICE_H
#define CARDSERVICE_H

#include <SdFat.h>
#include "User.h"

class CardService {
public:
    static void begin(int chipSelectPin);
    static void writeFileToCSV(const char* filename, User *user);
    static User* readFileFromCSV(const char* filename, int fingerId);
    static void deleteFileFromCSV(const char* filename);
    static void saveAttendance(const char* filename, User *user);
    static User** showAttendance(int& userCount, char* filename);

private:
    static SdFat sd;
};

#endif // CARDSERVICE_H