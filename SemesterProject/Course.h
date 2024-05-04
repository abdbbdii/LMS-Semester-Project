#pragma once

#include "Department.h"
#include "Utils.h"
#include <fstream>
#include <map>
#include <string>
#include "Storage.h"

using namespace std;

class Course {
private:
    int course_id, credit_hours;
    string course_name;
    Department department;

public:
    // Default constructor
    Course();

    // Parameterized constructor
    Course(const int& course_id, const string& course_name, const int& credit_hours, const Department& department);
    Course(const int& course_id);

    // Setters
    void set_course_id(const int& course_id);
    void set_course_name(const string& course_name);
    void set_credit_hours(const int& credit_hours);
    void set_department(const Department& department);

    // Getters
    int get_id() const;
    string get_course_name() const;
    int get_credit_hours() const;
    Department get_department() const;

    // Member functions
    void display() const;

    // Static member functions
    static map<int, Course> read();
    static void write(const map<int, Course>& courses);
    static void refresh(map<int, Course>& courses);
    static void display_headings();
    static Storage1<string> get_headings();
    static Storage1<int> get_widths();
    static void display_all(const map<int, Course>& courses);
    static bool create_course(map<int, Course>& courses, const map<string, Department>& departments);
    static bool update_course(map<int, Course>& courses, const map<string, Department>& departments);
};
// ------------------------------------- Ready for testing ------------------------------------- //