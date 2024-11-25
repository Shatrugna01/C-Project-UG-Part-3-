#include "User.h"
#include "Utility.h"
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;

// User class methods
User::User(const string& uname, const string& pass) : username(uname), password(pass) {
    if (uname.empty() || pass.empty()) {
        throw invalid_argument("Username and password cannot be empty.");
    }
}

string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

void User::setPassword(const string& pass) {
    if (pass.empty()) {
        throw invalid_argument("Password cannot be empty.");
    }
    password = pass;
}

void User::changePassword(const string& newPassword) {
    if (newPassword.empty()) {
        cout << "Error: Password cannot be empty." << endl;
        return;
    }
    password = newPassword;
    cout << "Password changed successfully!" << endl;
}
