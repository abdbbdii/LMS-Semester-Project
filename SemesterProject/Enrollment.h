#pragma once

#include "Course.h"
#include "Date.h"
#include "Student.h"
#include "Utils.h"
#include <fstream>
#include <string>
#include "Storage.h"
#include <map>

using namespace std;

class Enrollment {
private:
    int enrollment_id;
    Date enrollment_date;
    Student student;
    Course course;

public:
    // Default constructor
    Enrollment();

    // Parameterized constructor
    Enrollment(const int& enrollment_id, const Date& enrollment_date, const Student& student, const Course& course);
    Enrollment(const int& enrollment_id);

    // Setters
    void set_enrollment_id(const int& enrollment_id);
    void set_enrollment_date(const Date& enrollment_date);
    void set_student(const Student& student);
    void set_course(const Course& course);

    // Getters
    int get_enrollment_id() const;
    Date get_enrollment_date() const;
    Student get_student() const;
    Course get_course() const;

    // Member functions
    void display_enrollment() const;

    // Static member functions
    static void display_enrollment_heading();
    static map<int, Enrollment> read(const string& file_name, const map<int, Student>& students, const map<int, Course>& courses);
    static void write_enrollments(const string& file_name, const map<int, Enrollment>& enrollments);
    static void display_enrollments(const map<int, Enrollment>& enrollments);
};
// ------------------------------------- Ready for testing ------------------------------------- //