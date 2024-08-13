#include "User.h"

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

User::User() {}

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