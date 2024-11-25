#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>
#include <numeric>
#include "User.h"

using namespace std;

class Student : public User {
private:
    string batchName;

public:
    Student(const string& uname = "student_default", const string& pass = "student_default", const string& batch = "");
    void dashboard() override;
    void viewMarks();
    void reportIssue();
    void downloadMarksheet();
    string formatResult(const map<string, vector<pair<string, pair<int, int>>>>& semesterGrades);
};

#endif // STUDENT_H
