#pragma once

#include <string>
#include "Student.h"
#include "Course.h"
#include "Storage.h"
#include <map>

using namespace std;

class CourseFeedback {
private:
	int id, rating;
	string content;
	Student student;
	Course course;

public:
	// Default constructor
	CourseFeedback();

	// Parameterized constructor
	CourseFeedback(const int& feedback_ID, const int& rating, const string& content, const Student& student, const Course& course);
	CourseFeedback(const int& feedback_ID);

	// Setters
	void set_feedback_ID(const int& feedback_ID);
	void set_rating(const int& rating);
	void set_content(const string& content);
	void set_student(const Student& student);
	void set_course(const Course& course);

	// Getters
	int get_id() const;
	int get_rating() const;
	string get_content() const;
	Student get_student() const;
	Course get_course() const;

	// Member functions
	void display() const;

	// Static member functions
	static map<int, CourseFeedback> read(const map<int, Student>& students, const map<int, Course>& courses);
	static void write(const map<int, CourseFeedback>& course_feedbacks);
	static void refresh(map<int, CourseFeedback>& course_feedbacks, const map<int, Student>& students, const map<int, Course>& courses);
	static void display_headings();
	static Storage1<string> get_headings();
	static Storage1<int> get_widths();
	static void display_all(const map<int, CourseFeedback>& course_feedbacks);
	static bool create_course_feedback(const Student& student, const map<int, Course>& courses, map<int, CourseFeedback>& course_feedbacks);
};

// -------------------------------------------- Ready for testing -------------------------------------------- //
