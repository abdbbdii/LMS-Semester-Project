#include "CourseFeedback.h"

using namespace std;

// Default constructor
CourseFeedback::CourseFeedback() : id(-1), rating(0), content(""), student(Student()), course(Course()) {}

// Parameterized constructor
CourseFeedback::CourseFeedback(const int& id, const int& rating, const string& content, const Student& student, const Course& course) : id(id), rating(rating), content(content), student(student), course(course) {}
CourseFeedback::CourseFeedback(const int& id) {
	map<int, CourseFeedback> course_feedbacks = read({}, {});
	*this = course_feedbacks[id];
}

// Setters
void CourseFeedback::set_feedback_ID(const int& id) { this->id = id; }
void CourseFeedback::set_rating(const int& rating) { this->rating = rating; }
void CourseFeedback::set_content(const string& content) { this->content = content; }
void CourseFeedback::set_student(const Student& student) { this->student = student; }
void CourseFeedback::set_course(const Course& course) { this->course = course; }

// Getters
int CourseFeedback::get_id() const { return id; }
int CourseFeedback::get_rating() const { return rating; }
string CourseFeedback::get_content() const { return content; }
Student CourseFeedback::get_student() const { return student; }
Course CourseFeedback::get_course() const { return course; }

// Member functions
void CourseFeedback::write(const map<int, CourseFeedback>& course_feedbacks) {
	ofstream file("data/course_feedbacks.csv");
	if (file.is_open()) {
		for (auto& feedback : course_feedbacks) {
			file << feedback.second.get_id() << "," << feedback.second.get_rating() << "," << feedback.second.get_content() << "," << feedback.second.get_student().get_id() << "," << feedback.second.get_course().get_id() << endl;
		}
		file.close();
	}
}
map<int, CourseFeedback> CourseFeedback::read(const map<int, Student>& students, const map<int, Course>& courses) {
	map<int, CourseFeedback> course_feedbacks;
	course_feedbacks.clear();
	ifstream file("data/course_feedbacks.csv");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			if (tokens.size() == 5) {
				CourseFeedback feedback(stoi(tokens[0]), stoi(tokens[1]), tokens[2], students.at(stoi(tokens[3])), courses.at(stoi(tokens[4])));
				course_feedbacks[stoi(tokens[0])] = feedback;
			}
		}
		file.close();
	}
	return course_feedbacks;
}
void CourseFeedback::refresh(map<int, CourseFeedback>& course_feedbacks, const map<int, Student>& students, const map<int, Course>& courses) {
	write(course_feedbacks);
	course_feedbacks = read(students, courses);
}

Storage1<int> CourseFeedback::get_widths() {
	return { 5, 6, 50, 20, 20, 40 };
}
Storage1<string> CourseFeedback::get_headings() {
	return { "ID", "Rating", "Content", "Student Name", "Student Roll Number", "Course Name" };
}
void CourseFeedback::display_headings() {
	Utils::display_column(get_headings(), get_widths(), true);
}
void CourseFeedback::display() const {
	Utils::display_column({ to_string(id), to_string(rating), content, student.get_name(), student.get_roll_number(), course.get_course_name() }, get_widths());
}
void CourseFeedback::display_all(const map<int, CourseFeedback>& course_feedbacks) {
	display_headings();
	for (auto& feedback : course_feedbacks) {
		feedback.second.display();
	}
	Utils::display_column_separator(get_widths());
}
bool CourseFeedback::create_course_feedback(const Student& student, const map<int, Course>& courses, map<int, CourseFeedback>& course_feedbacks) {
	if (student.get_enrolled_courses().empty()) {
		Utils::print("You are not enrolled in any courses yet.", INFO);
		return false;
	}
	map<int, string> course_names;
	for (int i = 0; i < student.get_enrolled_courses().size(); i++) {
		course_names[student.get_enrolled_courses()[i].get_id()] = student.get_enrolled_courses()[i].get_course_name();
	}
	int selected_course_id = Utils::option_selector<int>(course_names, "Select a course", 50);
	CourseFeedback feedback;
	int feedback_id = course_feedbacks.empty() ? 1 : course_feedbacks.rbegin()->first + 1;
	feedback.set_feedback_ID(feedback_id);
	feedback.set_student(student);
	feedback.set_course(courses.at(selected_course_id));
	feedback.set_content(Utils::get_input("Enter your feedback", 30));

	course_feedbacks[feedback_id] = feedback;
}
// -------------------------------------------- Ready for testing -------------------------------------------- //