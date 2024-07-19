#include "HardwareSerial.h"
// #include "HardwareSerial.h"
// #include "WString.h"
// #include "time.h"
// #include "Arduino.h"

#include "User.h"

// Initialize static members
User *User::users[5] = {};

int User::userCount = 0;


// Parameterized constructor
User::User(int userId,
           String username,
           String password,
           int academic,
           String department,
           String rollNumber)
  : userId(userId),
    username(username),
    password(password),
    academic(academic),
    department(department),
    rollNumber(rollNumber) {}

void User::addUser(User *user) {
  if (User::userCount < 5) {
    User::users[User::userCount] = user;
    User::userCount++;
  }
}

void User::findAndPrintUser(int id) {
  for (int i = 0; i < User::userCount; i++) {
    if (users[i] != nullptr && users[i]->userId == id) {
      Serial.print(users[i]->getUsername());
      Serial.print(" -> ");
      Serial.print(users[i]->getAcademic());
      Serial.print(users[i]->getDepartment());
      Serial.print(" : ");
      Serial.print(users[i]->getRollNumber());
      return;
    }
  }

  Serial.print("No match found with id.");
  Serial.println(id);
}

void User::displayAllUsers() {
  for (int i = 0; i < User::userCount; i++) {
    Serial.print(User::users[i]->getUsername());
    Serial.print(" -> ");
    Serial.print(User::users[i]->getAcademic());
    Serial.print(" ");
    Serial.print(User::users[i]->getDepartment());
    Serial.print(" : ");
    Serial.print(User::users[i]->getRollNumber());
    Serial.println();
  }
}


// Getters and setters implementation
int User::getUserId() const {
  return userId;
}
void User::setUserId(int userId) {
  this->userId = userId;
}

String User::getUsername() const {
  return username;
}
void User::setUsername(String username) {
  this->username = username;
}

String User::getPassword() const {
  return password;
}
void User::setPassword(String password) {
  this->password = password;
}

int User::getAcademic() const {
  return academic;
}
void User::setAcademic(int academic) {
  this->academic = academic;
}

String User::getDepartment() const {
  return department;
}
void User::setDepartment(String department) {
  this->department = department;
}

String User::getRollNumber() const {
  return rollNumber;
}
void User::setRollNumber(String rollNumber) {
  this->rollNumber = rollNumber;
}