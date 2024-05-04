#pragma once

#include "Course.h"
#include "Student.h"
#include "Utils.h"
#include <fstream>
#include <string>
#include "Storage.h"
#include <map>

using namespace std;

class Grading {
private:
    int id;
    Student student;
    Course course;
    double marks;

public:
    // Default constructor
    Grading();

    // Parameterized constructor
    Grading(const int& id, const Student& student, const Course& course, double marks);

    // Setters
    void set_grading_id(const int& id);
    void set_student(const Student& student);
    void set_course(const Course& course);
    void set_marks(double marks);

    // Getters
    int get_id() const;
    Student get_student() const;
    Course get_course() const;
    double get_marks() const;

    // Member functions
    void display() const;

    // Static member functions
    static map<int, Grading> read(const map<int, Student>& students, const map<int, Course>& courses);
    static void write(const map<int, Grading>& gradings);
    static void refresh(map<int, Grading>& gradings, const map<int, Student>& students, const map<int, Course>& courses);
    static void display_headings();
    static Storage1<int> get_widths();
    static Storage1<string> get_headings();
    static void display_all(const map<int, Grading>& gradings);
    static bool grade_students(map<int, Grading>& gradings, const map<int, Student>& students, const map<int, Course>& courses);
};