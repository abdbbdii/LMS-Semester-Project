#pragma once

#include "Course.h"
#include "Faculty.h"
#include "Utils.h"
#include <fstream>
#include "Storage.h"
#include <map>

using namespace std;

class FacultyCourseAllocation {
private:
	int allocation_id;
	string semester;
	Faculty faculty;
	Course course;

public:
	// Default constructor
	FacultyCourseAllocation();

	// Parameterized constructor
	FacultyCourseAllocation(const int& allocation_id, const string& semester, const Faculty& faculty, const Course& course);
	FacultyCourseAllocation(const int& allocation_id);

	// Setters
	void set_allocation_id(const int& allocation_id);
	void set_semester(const string& semester);
	void set_faculty(const Faculty& faculty);
	void set_course(const Course& course);

	// Getters
	int get_allocation_id() const;
	string get_semester() const;
	Faculty get_faculty() const;
	Course get_course() const;

	// Member functions
	void display() const;

	// Static member functions
	static map<int, FacultyCourseAllocation> read(const map<int, Faculty>& faculties, const map<int, Course>& courses);
	static void write(const map<int, FacultyCourseAllocation>& faculty_course_allocations);
	static void display_headings();
	static void refresh(map<int, FacultyCourseAllocation>& faculty_course_allocations, const map<int, Faculty>& faculties, const map<int, Course>& courses);
	static void display_all(const map<int, FacultyCourseAllocation>& faculty_course_allocations);
	static void display_faculty_courses(const int& faculty_id, const map<int, FacultyCourseAllocation>& faculty_course_allocations);
	static map<int, Course> get_faculty_courses(const int& faculty_id, const map<int, FacultyCourseAllocation>& faculty_course_allocations);
	static Storage1<string> get_headings();
	static Storage1<int> get_column_widths();
	static bool allocate_course_to_faculty(map<int, FacultyCourseAllocation> &faculty_course_allocations, map<int, Faculty>& faculties, const map<int, Course>& courses);
	static bool deallocate_course_from_faculty(map<int, FacultyCourseAllocation> &faculty_course_allocations);
};
// ------------------------------------- Ready for testing ------------------------------------- //