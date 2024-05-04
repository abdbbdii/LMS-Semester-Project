#pragma once

#include "Course.h"
#include "Assignment.h"
#include "Club.h"
#include "Date.h"
#include "Department.h"
#include <fstream>
#include <iomanip>
#include <string>
#include "Storage.h"
#include <map>

using namespace std;

class Student {
	int id;
	string name, email, phone_number, roll_number, password;
	Department department;
	Date date_enrolled;
	Storage1<Course> enrolled_courses;
	Storage1<Assignment> submitted_assignments;
	Storage1<Club> joined_clubs;

public:
	// Default constructor
	Student();

	// Parameterized constructor
	Student(const int& id, const string& name, const string& email, const string& password, const string& phone_number, const string& roll_number, const Department& department, const Date& date_enrolled, const Storage1<Course>& enrolled_courses, const Storage1<Assignment>& submitted_assignments, const Storage1<Club>& joined_clubs);
	Student(const int& id);

	// Setters
	void set_id(const int& id);
	void set_name(const string& name);
	void set_email(const string& email);
	void set_password(const string& password);
	void set_phone_number(const string& phone_number);
	void set_roll_number(const string& roll_number);
	void set_department(const Department& department);
	void set_date_enrolled(const Date& date_enrolled);
	void set_enrolled_courses(const Storage1<Course>& enrolled_courses);
	void set_submitted_assignments(const Storage1<Assignment>& submitted_assignments);
	void set_joined_clubs(const Storage1<Club>& joined_clubs);

	// Getters
	int get_id() const;
	string get_name() const;
	string get_email() const;
	string get_password() const;
	string get_phone_number() const;
	string get_roll_number() const;
	Department get_department() const;
	Date get_date_enrolled() const;
	Storage1<Course> get_enrolled_courses() const;
	Storage1<Assignment> get_submitted_assignments() const;
	Storage1<Club> get_joined_clubs() const;

	// Member functions
	int find_enrolled_course(const int& course) const;
	int find_submitted_assignment(const int& assignment) const;
	int find_joined_club(const int& club) const;
	bool add_enrolled_course(const int& course, const map<int, Course>& courses);
	bool add_submitted_assignment(const int& id, const map<int, Assignment>& assignments);
	bool add_joined_club(const int& club, const map<int, Club>& clubs);
	bool remove_enrolled_course(const int& course);
	bool remove_submitted_assignment(const int& id);
	bool remove_joined_club(const int& club);
	void display_student() const;
	void display_enrolled_courses() const;
	void display_submitted_assignments() const;
	void display_joined_clubs() const;
	void display_club_events() const;
	bool change_student_password();
	bool change_student_phone_number();
	bool enroll_in_course(const map<int, Course>& courses);
	bool submit_assignment(const map<int, Assignment>& assignments);
	bool join_club(map<int, Club>& clubs);
	bool join_event(map<int, Event>& events);

	// Static member functions
	static map<int, Student> read(const map<int, Course>& courses, const map<int, Assignment>& assignments, const map<int, Club>& clubs);
	static void write_students(const map<int, Student>& students);
	static void refresh(map<int, Student>& students, const map<int, Course>& courses, const map<int, Assignment>& assignments, const map<int, Club>& clubs);
	static void display_student_heading();
	static Storage1<string> get_headings();
	static Storage1<int> get_widths();
	static void display_students(const map<int, Student>& students);
	static Student login(const map<int, Student>& students);
	static bool enroll_a_student_to_course(map<int, Student>& students, const map<int, Course> courses);
	static bool unenroll_a_student_from_course(map<int, Student>& students, const map<int, Course> courses);
	static bool admit_a_student(map<int, Student>& students, const map<string, Department>& departments);
};
// -------------------------------------------- Ready for testing -------------------------------------------- //