#pragma once

#include "Course.h"
#include "Date.h"
#include "Student.h"
#include "Utils.h"
#include <fstream>
#include "Storage.h"
#include <map>

using namespace std;

class Attendence {
    int id;
    Student student;
    Course course;
    Date date;
    char status;

public:
    // Default constructor
    Attendence();

    // Parameterized constructor
    Attendence(const int& id, const Student& student, const Course& course, const Date& date, const char& status);
    Attendence(const int& id);

    // Setters
    void set_id(const int& id);
    void set_student(const Student& student);
    void set_course(const Course& course);
    void set_date(const Date& date);
    void set_status(const char& status);

    // Getters
    int get_id() const;
    Student get_student() const;
    Course get_course() const;
    Date get_date() const;
    char get_status() const;

    // Member functions
    void display() const;
    bool change_status();

    // Static member functions
    static map<int, Attendence> read(const map<int, Student>& students, const map<int, Course>& courses);
    static void write(const map<int, Attendence>& attendences);
    static void refresh(map<int, Attendence>& attendences, const map<int, Student>& students, const map<int, Course>& courses);
    static void display_headings();
    static Storage1<string> get_headings();
    static Storage1<int> get_widths();
    static void display_all(const map<int, Attendence>& attendences);
    static bool mark_attendence(map<int, Attendence>& attendences, const map<int, Student>& students, const map<int, Course>& courses);
};