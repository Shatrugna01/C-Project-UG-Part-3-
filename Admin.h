#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>
#include <numeric>
#include "User.h"

using namespace std;
extern string Default_pass;

// Admin class
class Admin : public User {
public:
    
    Admin(const string& uname = "admin_default", const string& pass = "admin_default");
    void dashboard() override;
    void addTeacher();
    void addDean();
    void addBatch();
    void viewAllBatches();
    void viewAllTeachers();
    void viewDeans();
    void viewProgramDirectors();
    
};

#endif // ADMIN_H
