#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>
#include <numeric>

using namespace std;

// Base class for user roles
class User {
protected:
    string username;
    string password;

public:
    User(const string& uname = "default", const string& pass = "default");
    string getUsername() const;
    string getPassword() const;
    void setPassword(const string& pass);
    virtual void dashboard() = 0; // Pure virtual function
    void changePassword(const string& newPassword);
};

#endif // USER_H
