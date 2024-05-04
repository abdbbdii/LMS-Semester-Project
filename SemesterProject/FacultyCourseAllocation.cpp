#include "FacultyCourseAllocation.h"

using namespace std;

// Default constructor
FacultyCourseAllocation::FacultyCourseAllocation() : allocation_id(-1), semester(""), faculty(Faculty()), course(Course()) {}

// Parameterized constructor
FacultyCourseAllocation::FacultyCourseAllocation(const int& allocation_id, const string& semester, const Faculty& faculty, const Course& course) : allocation_id(allocation_id), semester(semester), faculty(faculty), course(course) {}
FacultyCourseAllocation::FacultyCourseAllocation(const int& allocation_id) {
	map<int, FacultyCourseAllocation> faculty_course_allocations = read({}, {});
	*this = faculty_course_allocations[allocation_id];
}

// Setters
void FacultyCourseAllocation::set_allocation_id(const int& allocation_id) { this->allocation_id = allocation_id; }
void FacultyCourseAllocation::set_semester(const string& semester) { this->semester = semester; }
void FacultyCourseAllocation::set_faculty(const Faculty& faculty) { this->faculty = faculty; }
void FacultyCourseAllocation::set_course(const Course& course) { this->course = course; }

// Getters
int FacultyCourseAllocation::get_allocation_id() const { return allocation_id; }
string FacultyCourseAllocation::get_semester() const { return semester; }
Faculty FacultyCourseAllocation::get_faculty() const { return faculty; }
Course FacultyCourseAllocation::get_course() const { return course; }

// Member functions
map<int, FacultyCourseAllocation> FacultyCourseAllocation::read(const map<int, Faculty>& faculties, const map<int, Course>& courses) {
	map<int, FacultyCourseAllocation> faculty_course_allocations;
	ifstream file("data/faculty_course_allocations.csv");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			if (tokens.size() == 4) {
				FacultyCourseAllocation faculty_course_allocation(stoi(tokens[0]), tokens[3], faculties.at(stoi(tokens[1])), courses.at(stoi(tokens[2])));
				faculty_course_allocations[faculty_course_allocation.get_allocation_id()] = faculty_course_allocation;
			}
		}
		file.close();
	}
	return faculty_course_allocations;
}
void FacultyCourseAllocation::write(const map<int, FacultyCourseAllocation>& faculty_course_allocations) {
	ofstream file("data/faculty_course_allocations.csv");
	if (file.is_open()) {
		for (auto& faculty_course_allocation : faculty_course_allocations) {
			file << faculty_course_allocation.second.get_allocation_id() << "," << faculty_course_allocation.second.get_faculty().get_id() << "," << faculty_course_allocation.second.get_course().get_id() << "," << faculty_course_allocation.second.get_semester() << endl;
		}
		file.close();
	}
}
void FacultyCourseAllocation::refresh(map<int, FacultyCourseAllocation>& faculty_course_allocations, const map<int, Faculty>& faculties, const map<int, Course>& courses) {
	write(faculty_course_allocations);
	faculty_course_allocations = read(faculties, courses);
}

Storage1<string> FacultyCourseAllocation::get_headings() {
	return { "ID", "Teacher Name", "Course Name", "Semester" };
}
Storage1<int> FacultyCourseAllocation::get_column_widths() {
	return { 5, 20, 40, 12 };
}
void FacultyCourseAllocation::display_headings() {
	Utils::display_column(get_headings(), get_column_widths(), true);
}
void FacultyCourseAllocation::display() const {
	Utils::display_column({ to_string(allocation_id), faculty.get_name(), course.get_course_name(), semester }, get_column_widths());
}
void FacultyCourseAllocation::display_all(const map<int, FacultyCourseAllocation>& faculty_course_allocations) {
	display_headings();
	for (auto& faculty_course_allocation : faculty_course_allocations) {
		faculty_course_allocation.second.display();
	}
	Utils::display_column_separator(get_column_widths());
}
void FacultyCourseAllocation::display_faculty_courses(const int& faculty_id, const map<int, FacultyCourseAllocation>& faculty_course_allocations) {
	display_headings();
	for (auto& faculty_course_allocation : faculty_course_allocations) {
		if (faculty_course_allocation.second.get_faculty().get_id() == faculty_id) {
			faculty_course_allocation.second.display();
		}
	}
	Utils::display_column_separator(get_column_widths());
}
map<int, Course> FacultyCourseAllocation::get_faculty_courses(const int& faculty_id, const map<int, FacultyCourseAllocation>& faculty_course_allocations) {
	map<int, Course> faculty_courses;
	for (auto& faculty_course_allocation : faculty_course_allocations) {
		if (faculty_course_allocation.second.get_faculty().get_id() == faculty_id) {
			faculty_courses[faculty_course_allocation.second.get_course().get_id()] = faculty_course_allocation.second.get_course();
		}
	}
	return faculty_courses;
}
bool FacultyCourseAllocation::allocate_course_to_faculty(map<int, FacultyCourseAllocation>& faculty_course_allocations, map<int, Faculty>& faculties, const map<int, Course>& courses) {
	if (faculties.empty() || courses.empty()) {
		Utils::print("No faculties or courses available to allocate.", INFO);
		_getch();
		return false;
	}
	map<int, string> course_names;
	for (auto& course : courses) {
		course_names[course.second.get_id()] = course.second.get_course_name();
	}
	int course_id = Utils::option_selector(course_names, "Select a course to allocate", 40);
	map<int, string> faculty_names;
	for (auto& faculty : faculties) {
		faculty_names[faculty.second.get_id()] = faculty.second.get_name();
	}
	int faculty_id = Utils::option_selector(faculty_names, "Select a faculty to allocate", 40);
semester_label:
	string semester = Utils::get_input("Enter the semester (Summer/Spring/Fall): ");
	if (semester != "Fall" && semester != "Spring" && semester != "Summer") {
		Utils::print("Invalid semester entered.", ERROR);
		_getch();
		Utils::remove_last_lines(2);
		goto semester_label;
	}
	int allocation_id = faculty_course_allocations.empty() ? 1 : faculty_course_allocations.rbegin()->first + 1;
	FacultyCourseAllocation faculty_course_allocation(allocation_id, semester, faculties.at(faculty_id), courses.at(course_id));

	faculty_course_allocations[allocation_id] = faculty_course_allocation;

	if (faculties[faculty_id].get_auth_level() < 2) {
		faculties[faculty_id].set_auth_level(2);
	}

	return true;
}
bool FacultyCourseAllocation::deallocate_course_from_faculty(map<int, FacultyCourseAllocation>& faculty_course_allocations) {
	if (faculty_course_allocations.empty()) {
		Utils::print("No faculty course allocations available to deallocate.", INFO);
		_getch();
		return false;
	}
	map<int, string> faculty_course_allocation_names;
	for (auto& faculty_course_allocation : faculty_course_allocations) {
		faculty_course_allocation_names[faculty_course_allocation.second.get_allocation_id()] = faculty_course_allocation.second.get_faculty().get_name() + " - " + faculty_course_allocation.second.get_course().get_course_name();
	}
	int allocation_id = Utils::option_selector(faculty_course_allocation_names, "Select allocation to deallocate", 50);
	faculty_course_allocations.erase(allocation_id);
	return true;
}

// ------------------------------------------------------------------ Ready for testing ------------------------------------------------------------------ //