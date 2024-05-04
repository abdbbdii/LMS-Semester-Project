#include "Student.h"

using namespace std;

// Default constructor
Student::Student() : id(-1), name(""), email(""), phone_number(""), roll_number(""), department(Department()), date_enrolled(Date()), enrolled_courses({}), submitted_assignments({}), joined_clubs({}) {}

// Parameterized constructor
Student::Student(const int& id, const string& name, const string& email, const string& password, const string& phone_number, const string& roll_number, const Department& department, const Date& date_enrolled, const Storage1<Course>& enrolled_courses, const Storage1<Assignment>& submitted_assignments, const Storage1<Club>& joined_clubs) : id(id), name(name), email(email), password(password), phone_number(phone_number), roll_number(roll_number), department(department), date_enrolled(date_enrolled), enrolled_courses(enrolled_courses), submitted_assignments(submitted_assignments), joined_clubs(joined_clubs) {}
Student::Student(const int& id) {
	map<int, Student> students = read({}, {}, {});
	*this = students[id];
}

// Setters
void Student::set_id(const int& id) { this->id = id; }
void Student::set_name(const string& name) { this->name = name; }
void Student::set_email(const string& email) { this->email = email; }
void Student::set_password(const string& password) { this->password = password; }
void Student::set_phone_number(const string& phone_number) { this->phone_number = phone_number; }
void Student::set_roll_number(const string& roll_number) { this->roll_number = roll_number; }
void Student::set_department(const Department& department) { this->department = department; }
void Student::set_date_enrolled(const Date& date_enrolled) { this->date_enrolled = date_enrolled; }
void Student::set_enrolled_courses(const Storage1<Course>& enrolled_courses) { this->enrolled_courses = enrolled_courses; }
void Student::set_submitted_assignments(const Storage1<Assignment>& submitted_assignments) { this->submitted_assignments = submitted_assignments; }
void Student::set_joined_clubs(const Storage1<Club>& joined_clubs) { this->joined_clubs = joined_clubs; }

// Getters
int Student::get_id() const { return id; }
string Student::get_name() const { return name; }
string Student::get_email() const { return email; }
string Student::get_password() const { return password; }
string Student::get_phone_number() const { return phone_number; }
string Student::get_roll_number() const { return roll_number; }
Department Student::get_department() const { return department; }
Date Student::get_date_enrolled() const { return date_enrolled; }
Storage1<Course> Student::get_enrolled_courses() const { return enrolled_courses; }
Storage1<Assignment> Student::get_submitted_assignments() const { return submitted_assignments; }
Storage1<Club> Student::get_joined_clubs() const { return joined_clubs; }

// Member functions
int Student::find_enrolled_course(const int& course_id) const {
	for (int i = 0; i < enrolled_courses.size(); i++) {
		if (enrolled_courses[i].get_id() == course_id) {
			return i;
		}
	}
	return -1;
}
int Student::find_submitted_assignment(const int& assignment_id) const {
	for (int i = 0; i < submitted_assignments.size(); i++) {
		if (submitted_assignments[i].get_id() == assignment_id) {
			return i;
		}
	}
	return -1;
}
int Student::find_joined_club(const int& club_id) const {
	for (int i = 0; i < joined_clubs.size(); i++) {
		if (joined_clubs[i].get_id() == club_id) {
			return i;
		}
	}
	return -1;
}
bool Student::add_enrolled_course(const int& course_id, const map<int, Course>& courses) {
	if (find_enrolled_course(course_id) != -1) {
		return false;
	}
	enrolled_courses.push_back(courses.at(course_id));
	return true;
}
bool Student::add_submitted_assignment(const int& assignment_id, const map<int, Assignment>& assignments) {
	if (find_submitted_assignment(assignment_id) != -1) {
		return false;
	}
	submitted_assignments.push_back(assignments.at(assignment_id));
	return true;
}
bool Student::add_joined_club(const int& club_id, const map<int, Club>& clubs) {
	if (find_joined_club(club_id) != -1) {
		return false;
	}
	joined_clubs.push_back(clubs.at(club_id));
	return true;
}
bool Student::remove_enrolled_course(const int& course_id) {
	int index = find_enrolled_course(course_id);
	if (index == -1) {
		return false;
	}
	enrolled_courses.erase(enrolled_courses.begin() + index);
	return true;
}
bool Student::remove_submitted_assignment(const int& id) {
	int index = find_submitted_assignment(id);
	if (index == -1) {
		return false;
	}
	submitted_assignments.erase(submitted_assignments.begin() + index);
	return true;
}
bool Student::remove_joined_club(const int& club_id) {
	int index = find_joined_club(club_id);
	if (index == -1) {
		return false;
	}
	joined_clubs.erase(joined_clubs.begin() + index);
	return true;
}
void Student::display_enrolled_courses() const {
	Course::display_headings();
	for (int i = 0; i < enrolled_courses.size(); i++) {
		enrolled_courses[i].display();
	}
	Utils::display_column_separator(Course::get_widths());
}
void Student::display_submitted_assignments() const {
	Assignment::display_headings();
	for (int i = 0; i < submitted_assignments.size(); i++) {
		submitted_assignments[i].display();
	}
	Utils::display_column_separator(Assignment::get_widths());
}
void Student::display_joined_clubs() const {
	Club::display_headings();
	for (int i = 0; i < joined_clubs.size(); i++) {
		joined_clubs[i].display();
	}
	Utils::display_column_separator(Club::get_widths());
}
void Student::display_club_events() const {
	for (int i = 0; i < joined_clubs.size(); i++) {
		Utils::print(joined_clubs[i].get_name() + " events:");
		Event::display_headings();
		for (int j = 0; j < joined_clubs[i].get_events().size(); j++) {
			joined_clubs[i].get_events()[i].display_event();
		}
		Utils::display_column_separator(Event::get_widths());
		cout << endl;
	}
}

map<int, Student> Student::read(const map<int, Course>& courses, const map<int, Assignment>& assignments, const map<int, Club>& clubs) {
	map<int, Student> students;
	students.clear();
	ifstream file("data/students.csv");
	if (file.is_open()) {
		students.clear();
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			if (tokens.size() == 11) {
				Storage1<Course> enrolled_courses;
				Storage1<Assignment> submitted_assignments;
				Storage1<Club> joined_clubs;

				Storage1<string> enrolled_course_ids = Utils::split(tokens[8], ';');
				if (enrolled_course_ids[0] != "")
					for (int i = 0; i < enrolled_course_ids.size(); i++)
						enrolled_courses.push_back(courses.at(stoi(enrolled_course_ids[i])));

				Storage1<string> submitted_assignment_ids = Utils::split(tokens[9], ';');
				if (submitted_assignment_ids[0] != "")
					for (int i = 0; i < submitted_assignment_ids.size(); i++)
						submitted_assignments.push_back(assignments.at(stoi(submitted_assignment_ids[i])));

				Storage1<string> joined_club_ids = Utils::split(tokens[10], ';');
				if (joined_club_ids[0] != "")
					for (int i = 0; i < joined_club_ids.size(); i++)
						joined_clubs.push_back(clubs.at(stoi(joined_club_ids[i])));

				students[stoi(tokens[0])] = Student(stoi(tokens[0]), tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], Department(tokens[6]), Date(tokens[7]), enrolled_courses, submitted_assignments, joined_clubs);
			}
		}
		file.close();
	}
	return students;
}
void Student::write_students(const map<int, Student>& students) {
	ofstream file("data/students.csv");
	if (file.is_open()) {
		for (auto& student : students) {
			file << student.second.get_id() << "," << student.second.get_name() << "," << student.second.get_email() << "," << student.second.get_password() << "," << student.second.get_phone_number() << "," << student.second.get_roll_number() << "," << student.second.get_department().get_alias() << "," << student.second.get_date_enrolled() << ",";
			for (int i = 0; i < student.second.get_enrolled_courses().size(); i++) {
				file << student.second.get_enrolled_courses()[i].get_id();
				if (i < student.second.get_enrolled_courses().size() - 1) {
					file << ";";
				}
			}
			file << ",";
			for (int i = 0; i < student.second.get_submitted_assignments().size(); i++) {
				file << student.second.get_submitted_assignments()[i].get_id();
				if (i < student.second.get_submitted_assignments().size() - 1) {
					file << ";";
				}
			}
			file << ",";
			for (int i = 0; i < student.second.get_joined_clubs().size(); i++) {
				file << student.second.get_joined_clubs()[i].get_id();
				if (i < student.second.get_joined_clubs().size() - 1) {
					file << ";";
				}
			}
			file << endl;
		}
		file.close();
	}
}
void Student::refresh(map<int, Student>& students, const map<int, Course>& courses, const map<int, Assignment>& assignments, const map<int, Club>& clubs) {
	write_students(students);
	students = read(courses, assignments, clubs);
}

Storage1<string> Student::get_headings() {
	return { "Name", "Email", "Phone Number", "Roll Number", "Department" };
}
Storage1<int> Student::get_widths() {
	return { 30, 30, 15, 15, 30 };
}
void Student::display_student_heading() {
	Utils::display_column(get_headings(), get_widths(), true);
}
void Student::display_student() const {
	Utils::display_column({ name, email, phone_number, roll_number, department.get_name() }, get_widths());
}
void Student::display_students(const map<int, Student>& students) {
	display_student_heading();
	for (auto& student : students) {
		student.second.display_student();
	}
	Utils::display_column_separator(get_widths());
}

Student Student::login(const map<int, Student>& students) {
	string email = Utils::get_input("Enter email: ");
	string password = Utils::get_input("Enter password: ", 20, false, true);
	for (auto& student : students) {
		if (student.second.get_email() == email && student.second.get_password() == password) {
			return student.second;
		}
	}
	return Student();
}
bool Student::change_student_password() {
	string old_password = Utils::get_input("Enter old password: ", 30, false, true);
	if (old_password != get_password()) {
		Utils::print("Incorrect password.", INFO);
		_getch();
		return false;
	}
	string new_password = Utils::get_input("Enter new password: ", 30, false, true);
	if (!Check::is_valid(new_password, "password")) {
		Utils::print("Password must be at least 8 characters long and should have valid characters.", INFO);
		_getch();
		return false;
	}
	string confirm_password = Utils::get_input("Confirm new password: ", 30, false, true);
	if (new_password != confirm_password) {
		Utils::print("Passwords do not match.", INFO);
		_getch();
		return false;
	}
	set_password(new_password);
	return true;
}
bool Student::change_student_phone_number() {
	string phone_number = Utils::get_input("Enter new phone number: ", 25);
	if (Check::is_valid(phone_number, "phone_number")) {
		set_phone_number(phone_number);
		return true;
	}
	Utils::print("Invalid phone number.", INFO);
	_getch();
	return false;
}
bool Student::enroll_in_course(const map<int, Course>& courses) {
	if (courses.empty()) {
		Utils::print("No courses available for enrollment.", INFO);
		_getch();
		return false;
	}
	map<int, string> available_courses;
	for (auto& course : courses) {
		if (course.second.get_department().get_alias() == department.get_alias()) {
			available_courses[course.first] = course.second.get_course_name();
		}
	}
	if (available_courses.empty()) {
		Utils::print("No courses available for enrollment.", INFO);
		_getch();
		return false;
	}
	int id = Utils::option_selector<int>(available_courses, "Select a course to enroll in: ", 40);
	if (add_enrolled_course(id, courses)) {
		return true;
	}
	else {
		Utils::print("You are already enrolled in this course.", INFO);
		_getch();
		return false;
	}
}
bool Student::submit_assignment(const map<int, Assignment>& assignments) {
	map<int, string> available_assignments;
	for (auto& assignment : assignments) {
		if (find_enrolled_course(assignment.second.get_id()) != -1) {
			available_assignments[assignment.first] = assignment.second.get_description();
		}
	}
	if (available_assignments.empty()) {
		Utils::print("No assignments :)", INFO);
		_getch();
		return false;
	}
	int id = Utils::option_selector<int>(available_assignments, "Select an assignment to submit: ", 40);
	if (find_submitted_assignment(id) != -1) {
		Utils::print("Assignment already submitted.", INFO);
		_getch();
		return false;
	}
	if (assignments.at(id).get_due_date() < Date()) {
		Utils::print("Assignment due date has passed.", INFO);
		_getch();
		return false;
	}
	if (add_submitted_assignment(id, assignments)) {
		return true;
	}
	else {
		Utils::print("Assignment already submitted.", INFO);
		_getch();
		return false;
	}
}
bool Student::join_club(map<int, Club>& clubs) {
	if (clubs.empty()) {
		Utils::print("No clubs available to join.", INFO);
		_getch();
		return false;
	}

	map<int, string> available_clubs;
	for (auto& club : clubs) {
		available_clubs[club.first] = club.second.get_name();
	}

	int id = Utils::option_selector<int>(available_clubs, "Select a club to join: ", 40);
	if (add_joined_club(id, clubs)) {
		clubs[id].add_member_id(get_id());
		return true;
	}
	else {
		Utils::print("You are already a member of this club.", INFO);
		_getch();
		return false;
	}
}
bool Student::join_event(map<int, Event>& events) {
	map<int, string> available_events;
	for (int i = 0; i < joined_clubs.size(); i++) {
		for (int j = 0; j < joined_clubs[i].get_events().size(); j++) {
			available_events[joined_clubs[i].get_events()[j].get_event_id()] = joined_clubs[i].get_events()[j].get_title() + " by " + joined_clubs[i].get_name();
		}
	}
	if (available_events.empty()) {
		Utils::print("No events available to join.", INFO);
		_getch();
		return false;
	}
	int id = Utils::option_selector<int>(available_events, "Select an event to join: ", 60);
	events[id].add_attendee(get_id());
	return true;
}
bool Student::enroll_a_student_to_course(map<int, Student>& students, const map<int, Course> courses) {
	map <int, string> available_students;
	for (auto& student : students) {
		available_students[student.first] = student.second.get_name();
	}
	int selected_student_id = Utils::option_selector<int>(available_students, "Select a student to enroll: ", 30);
	if (students[selected_student_id].add_enrolled_course(selected_student_id, courses)) {
		return true;
	}
	else {
		Utils::print("Student is already enrolled in this course.", INFO);
		_getch();
		return false;
	}
}
bool Student::unenroll_a_student_from_course(map<int, Student>& students, const map<int, Course> courses) {
	map <int, string> available_students;
	for (auto& student : students) {
		available_students[student.first] = student.second.get_name();
	}
	int selected_student_id = Utils::option_selector<int>(available_students, "Select a student to unenroll: ", 30);
	if (students[selected_student_id].remove_enrolled_course(selected_student_id)) {
		return true;
	}
	else {
		Utils::print("Student is not enrolled in this course.", INFO);
		_getch();
		return false;
	}
}

bool Student::admit_a_student(map<int, Student>& students, const map<string, Department>& departments) {

	Student student;

	student.set_id(students.rbegin()->first + 1);

name_again:
	string name = Utils::get_input("Enter student name: ", 30);
	if (!Check::is_valid(name, "name")) {
		Utils::print("Invalid name.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto name_again;
	}
	student.set_name(name);

email_again:
	string email = Utils::get_input("Enter student email: ", 30);
	if (!Check::is_valid(email, "email")) {
		Utils::print("Invalid email.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto email_again;
	}
	for (auto& existing_student : students) {
		if (existing_student.second.get_email() == email) {
			Utils::print("Email already exists.", INFO);
			_getch();
			Utils::remove_last_lines(2);
			goto email_again;
		}
	}
	student.set_email(email);

password_again:
	string password = Utils::get_input("Enter student password: ", 30, false, true);
	if (!Check::is_valid(password, "password")) {
		Utils::print("Password must be at least 8 characters long and should have valid characters.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto password_again;
	}
	student.set_password(password);

phone_number_again:
	string phone_number = Utils::get_input("Enter student phone number: ", 30);
	if (!Check::is_valid(phone_number, "phone_number")) {
		Utils::print("Invalid phone number.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto phone_number_again;
	}
	student.set_phone_number(phone_number);

roll_number_again:
	string roll_number = Utils::get_input("Enter student roll number: ", 30);
	if (!Check::is_valid(roll_number, "roll_number")) {
		Utils::print("Invalid roll number.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto roll_number_again;
	}
	for (auto& existing_student : students) {
		if (existing_student.second.get_roll_number() == roll_number) {
			Utils::print("Roll number already exists.", INFO);
			_getch();
			Utils::remove_last_lines(2);
			goto roll_number_again;
		}
	}
	student.set_roll_number(roll_number);

	map<string, string> department_names;
	for (auto& department : departments) {
		department_names[department.second.get_alias()] = department.second.get_name();
	}
	string department_alias = Utils::option_selector<string>(department_names, "Select student department: ", 30);
	student.set_department(departments.at(department_alias));

	student.set_date_enrolled(Date());

	students[student.get_id()] = student;

	return true;
}
// ---------------------------- Ready for testing ---------------------------- //
