#ifndef DEAN_H
#define DEAN_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>
#include <numeric>
#include "User.h"

using namespace std;

// Dean class
class Dean : public User {
private:
    vector<string> programs;

public:
    Dean(const string& uname = "dean_default", const string& pass = "dean_default");
    void dashboard() override;
    void assignProgramDirector(const string& batchName, const string& programDirector);
    void viewAllTeachersAndProgramDirectors();
    void declareResults(const string& batchName);
    void viewAllBatches();
};

#endif // DEAN_H
