#ifndef TEACHER_H
#define TEACHER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "User.h"
#include "Utility.h"

using namespace std;

class Teacher : public User {
private:
    bool isProgramDirector;                 // Indicates if the teacher is also a Program Director
    vector<string> assignedBatches;         // List of batches assigned to the teacher
    map<string, string> batchSubjects;      // Maps Batch -> Subject assigned to the teacher
    map<string, string> programDirectorBatches; // Maps Batch -> Program Director

public:
    // Constructor
    Teacher(const string& uname = "default", const string& pass = "default", bool isPD = false);

    // Dashboard
    void dashboard() override;

    // Functionalities for Teacher
    void addMarksToBatch();                           // Add marks for assigned batches and subjects
    void resolveStudentIssues();                     // Resolve student issues for a batch
    void viewAssignedSubjects() const;               // View assigned subjects
    void submitMarksToProgramDirector();             // Submit marks to the Program Director

    // Functionalities for Program Director
    void assignTeacherToBatch(const string& teacherUsername, const string& batchName, const string& subject, const string& semester); 
    void viewAssignedAndUnassignedTeachers();        // View all teachers and their assignments
    void viewAssignedBatches() const;                // View batches assigned to the Program Director
    void generateBatchReport(const string& batchName); // Generate and save batch report

    // Utility Functions
    bool getIsProgramDirector() const { return isProgramDirector; } // Check if the teacher is a Program Director
    vector<string> getAssignedBatches() const { return assignedBatches; }
    const map<string, string>& getProgramDirectorBatches() const { return programDirectorBatches; }
    const map<string, string>& getBatchSubjects() const { return batchSubjects; }
    void assignBatchAndSubject(const string& batchName, const string& subject); // Assign batch and subject to teacher
};

#endif // TEACHER_H

