#include "Course.h"

using namespace std;

// Default constructor
Course::Course() : course_id(-1), credit_hours(0), course_name("") {}

// Parameterized constructor
Course::Course(const int& course_id, const string& course_name, const int& credit_hours, const Department& department) : course_id(course_id), course_name(course_name), credit_hours(credit_hours), department(department) {}
Course::Course(const int& course_id) {
	map<int, Course> courses = read();
	*this = courses[course_id];
}

// Setters
void Course::set_course_id(const int& course_id) { this->course_id = course_id; }
void Course::set_course_name(const string& course_name) { this->course_name = course_name; }
void Course::set_credit_hours(const int& credit_hours) { this->credit_hours = credit_hours; }
void Course::set_department(const Department& department) { this->department = department; }

// Getters
int Course::get_id() const { return course_id; }
string Course::get_course_name() const { return course_name; }
int Course::get_credit_hours() const { return credit_hours; }
Department Course::get_department() const { return department; }

// Member functions
map<int, Course> Course::read() {
	map<int, Course> courses;
	courses.clear();
	ifstream file("data/courses.csv");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			if (tokens.size() == 4) {
				Course course(stoi(tokens[0]), tokens[1], stoi(tokens[2]), Department(tokens[3]));
				courses[course.get_id()] = course;
			}
		}
		file.close();
	}
	return courses;
}
void Course::write(const map<int, Course>& courses) {
	ofstream file("data/courses.csv");
	if (file.is_open()) {
		for (auto& course : courses) {
			file << course.second.get_id() << "," << course.second.get_course_name() << "," << course.second.get_credit_hours() << "," << course.second.get_department().get_alias() << endl;
		}
		file.close();
	}
}
void Course::refresh(map<int, Course>& courses) {
	write(courses);
	courses = read();
}

Storage1<string> Course::get_headings() {
	return { "ID", "Course Name", "Credit Hours", "Department" };
}
Storage1<int> Course::get_widths() {
	return { 5, 40, 15, 10 };
}
void Course::display_headings() {
	Utils::display_column(get_headings(), get_widths(), true);
}
void Course::display() const {
	Utils::display_column({ to_string(course_id), course_name, to_string(credit_hours), department.get_alias() }, get_widths());
}
void Course::display_all(const map<int, Course>& courses) {
	display_headings();
	for (auto& course : courses) {
		course.second.display();
	}
	Utils::display_column_separator(get_widths());
}
bool Course::create_course(map<int, Course>& courses, const map<string, Department>& departments) {
	map<string, string> deparment_names;
	for (auto& department : departments) {
		deparment_names[department.second.get_alias()] = department.second.get_name();
	}
	if (departments.empty()) {
		Utils::print("No departments found. Please create a department first.", INFO);
		_getch();
		return false;
	}
	string selected_department = Utils::option_selector<string>(deparment_names, "Select Department");
	Course course;
	course.set_department(Department(selected_department));
	course.set_course_id(courses.empty() ? 1 : courses.rbegin()->first + 1);
	course.set_course_name(Utils::get_input("Enter Course Name", 30));
create_label:
	string credit_hours = Utils::get_input("Enter Credit Hours", 30);
	if (!Check::is_valid(credit_hours, "int")) {
		Utils::print("Credit hours must be a number.", ERROR);
		_getch();
		Utils::remove_last_lines(2);
		goto create_label;
	}
	course.set_credit_hours(stoi(credit_hours));
	courses[course.get_id()] = course;
}
bool Course::update_course(map<int, Course>& courses, const map<string, Department>& departments) {
	if (courses.empty()) {
		Utils::print("No courses found. Please create a course first.", INFO);
		_getch();
		return false;
	}
	map<int, string> course_names;
	for (auto& course : courses) {
		course_names[course.first] = course.second.get_course_name();
	}
	int selected_course_id = Utils::option_selector<int>(course_names, "Select Course");
	Course course = courses[selected_course_id];
	map<string, string> deparment_names;
	for (auto& department : departments) {
		deparment_names[department.second.get_alias()] = department.second.get_name();
	}
	string selected_department;
update_label:
	system("cls");
	string credit_hours;
	switch (Utils::create_menu({ "Update " + courses[selected_course_id].get_course_name(), "Course Name", "Credit Hours", "Department" }, true, true, 50)) {
	case 1:
		course.set_course_name(Utils::get_input("Enter Course Name", 30));
		break;
	case 2:
		credit_hours = Utils::get_input("Enter Credit Hours", 30);
		if (!Check::is_valid(credit_hours, "int")) {
			Utils::print("Credit hours must be a number.", ERROR);
			_getch();
			Utils::remove_last_lines(2);
			goto update_label;
		}
		course.set_credit_hours(stoi(credit_hours));
		break;
	case 3:
		selected_department = Utils::option_selector<string>(deparment_names, "Select Department");
		course.set_department(Department(selected_department));
		break;
	case -1:
		courses[selected_course_id] = course;
		return true;
	}
	goto update_label;
}

// -------------------------------------------- Ready for testing -------------------------------------------- //