#include "Enrollment.h"

using namespace std;

// Default constructor
Enrollment::Enrollment() : enrollment_id(0), enrollment_date(Date()), student(Student()), course(Course()) {}

// Parameterized constructor
Enrollment::Enrollment(const int& enrollment_id, const Date& enrollment_date, const Student& student, const Course& course) : enrollment_id(enrollment_id), enrollment_date(enrollment_date), student(student), course(course) {}
Enrollment::Enrollment(const int& enrollment_id) {
    map<int, Enrollment> enrollments = read("data/enrollments.csv", {}, {});
    *this = enrollments[enrollment_id];
}

// Setters
void Enrollment::set_enrollment_id(const int& enrollment_id) { this->enrollment_id = enrollment_id; }
void Enrollment::set_enrollment_date(const Date& enrollment_date) { this->enrollment_date = enrollment_date; }
void Enrollment::set_student(const Student& student) { this->student = student; }
void Enrollment::set_course(const Course& course) { this->course = course; }

// Getters
int Enrollment::get_enrollment_id() const { return enrollment_id; }
Date Enrollment::get_enrollment_date() const { return enrollment_date; }
Student Enrollment::get_student() const { return student; }
Course Enrollment::get_course() const { return course; }

// Member functions
map<int, Enrollment> Enrollment::read(const string& file_name, const map<int, Student>& students, const map<int, Course>& courses) {
    map<int, Enrollment> enrollments;
    ifstream file(file_name);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Storage1<string> tokens = Utils::split(line, ',');
            if (tokens.size() == 4) {
                Enrollment enrollment(stoi(tokens[0]), Date(tokens[1]), students.at(stoi(tokens[2])), courses.at(stoi(tokens[3])));
                enrollments[enrollment.get_enrollment_id()] = enrollment;
            }
        }
        file.close();
    }
    return enrollments;
}
void Enrollment::write_enrollments(const string& file_name, const map<int, Enrollment>& enrollments) {
    ofstream file(file_name);
    if (file.is_open()) {
        for (auto& enrollment : enrollments) {
            file << enrollment.second.get_enrollment_id() << "," << enrollment.second.get_enrollment_date() << "," << enrollment.second.get_student().get_id() << "," << enrollment.second.get_course().get_id() << endl;
        }
        file.close();
    }
}

Storage1<int> enrollment_column_widths = {5, 40, 20, 20, 20};
void Enrollment::display_enrollment_heading() {
    Utils::display_column({ "ID", "Course Name", "Student Roll Number", "Student Name", "Enrollment Date" }, enrollment_column_widths, true);
}
void Enrollment::display_enrollment() const {
    Utils::display_column({to_string(enrollment_id), course.get_course_name(), student.get_roll_number(), student.get_name(), enrollment_date.toString()}, enrollment_column_widths);
}
void Enrollment::display_enrollments(const map<int, Enrollment>& enrollments) {
    display_enrollment_heading();
    for (auto& enrollment : enrollments) {
        enrollment.second.display_enrollment();
    }
    Utils::display_column_separator(enrollment_column_widths);
}
// ------------------------------------- Ready for testing ------------------------------------- //