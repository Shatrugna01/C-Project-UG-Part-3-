#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "User.h"
#include "Admin.h"
#include "Teacher.h"
#include "Dean.h"
#include "Student.h"

using namespace std;
class Teacher; // Forward declaration

// Global variables
extern map<string, Admin> admins;
extern map<string, Teacher> teachers;
extern map<string, Dean> deans;

// Function declarations
void saveData();
void loadData();
vector<string> loadBatchData(const string& batchName);
void saveBatchData(const string& batchName, const vector<string>& data);
vector<string> loadBatchIssues(const string& batchName);
void saveBatchIssues(const string& batchName, const vector<string>& issues);
vector<string> loadBatchNames();
void saveBatchNames(const vector<string>& batchNames);
void saveProgramDirectorAssignments(const map<string, string>& batchAssignments);
map<string, string> loadProgramDirectorAssignments();
void saveResultStatuses(const map<string, bool>& resultStatuses);
map<string, bool> loadResultStatuses();
void viewReport(const string& batchName);

// Admin utilities
void viewAllBatches();
void viewAllTeachers();
void viewDeans();
void viewProgramDirectors();

#endif // UTILITY_H

