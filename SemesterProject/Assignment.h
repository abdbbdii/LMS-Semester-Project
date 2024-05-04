#pragma once

#include "Course.h"
#include "Date.h"
#include "FacultyCourseAllocation.h"
#include "Storage.h"
#include <fstream>
#include <string>
#include <map>

using namespace std;

class Assignment {
    int id;
    Date due_date;
    string description;
    Course course;

public:
    // Default constructor
    Assignment();

    // Parameterized constructor
    Assignment(const int& id, const Date& due_date, const string& description, const Course& course);
    Assignment(const int& id);

    // Setters
    void set_id(const int& id);
    void set_due_date(const Date& due_date);
    void set_description(const string& description);
    void set_course(const Course& course);

    // Getters
    int get_id() const;
    string get_description() const;
    Date get_due_date() const;
    Course get_course() const;

    // Member functions
    void display() const;
    bool change_due_date();
    bool change_description();
    bool change_course(const map<int, Course>& courses);

    // Static member funstions
    static map<int, Assignment> read(const map<int, Course>& courses);
    static void write(const map<int, Assignment>& assignment);
    static void refresh(map<int, Assignment>& assignment, const map<int, Course>& courses);
    static void display_headings();
    static Storage1<string> get_headings();
    static Storage1<int> get_widths();
    static void display_all(const map<int, Assignment>& assignment);
    static bool create_assignment(map<int, Assignment>& assignments, const map<int, Course>& courses);
    static bool edit_assignment(map<int, Assignment>& assignments, const map<int, Course>& courses);
};
// -------------------------------------------- Ready for testing -------------------------------------------- //
