#ifndef USER_H
#define USER_H

#include <Arduino.h>

class User {
private:
  int userId;
  String username;
  String password;
  int academic;
  String department;
  String rollNumber;

public:

  User(int userId, String username, String password, int academic, String department, String rollNumber);

  static int userCount;        
  static User *users[5];
  static void addUser(User *user);
  static void findAndPrintUser(int id);
  static void displayAllUsers();

// Getters and setters
  int getUserId() const;
  void setUserId(int userId);

  String getUsername() const;
  void setUsername(String username);

  String getPassword() const;
  void setPassword(String password);

  int getAcademic() const;
  void setAcademic(int academic);

  String getDepartment() const;
  void setDepartment(String department);

  String getRollNumber() const;
  void setRollNumber(String rollNumber);
};
#endif
