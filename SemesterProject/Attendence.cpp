#include "Attendence.h"

using namespace std;

// Default constructor
Attendence::Attendence() : id(-1), student(Student()), course(Course()), date(Date()), status('A') {}

// Parameterized constructor
Attendence::Attendence(const int& id, const Student& student, const Course& course, const Date& date, const char& status) : id(id), student(student), course(course), date(date), status(status) {}
Attendence::Attendence(const int& id) {
    map<int, Attendence> attendences = read({}, {});
    *this = attendences[id];
}

// Setters
void Attendence::set_id(const int& id) { this->id = id; }
void Attendence::set_student(const Student& student) { this->student = student; }
void Attendence::set_course(const Course& course) { this->course = course; }
void Attendence::set_date(const Date& date) { this->date = date; }
void Attendence::set_status(const char& status) { this->status = status; }

// Getters
int Attendence::get_id() const { return id; }
Student Attendence::get_student() const { return student; }
Course Attendence::get_course() const { return course; }
Date Attendence::get_date() const { return date; }
char Attendence::get_status() const { return status; }

// Member functions
map<int, Attendence> Attendence::read(const map<int, Student>& students, const map<int, Course>& courses) {
    map<int, Attendence> attendences;
    attendences.clear();
    ifstream file("data/attendences.csv");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Storage1<string> tokens = Utils::split(line, ',');
            if (tokens.size() == 5) {
                Attendence attendence(stoi(tokens[0]), students.at(stoi(tokens[1])), courses.at(stoi(tokens[2])), Date(tokens[3]), tokens[4][0]);
                attendences[attendence.get_id()] = attendence;
            }
        }
        file.close();
    }
    return attendences;
}
void Attendence::write(const map<int, Attendence>& attendences) {
    ofstream file("data/attendences.csv");
    if (file.is_open()) {
        for (auto& attendence : attendences) {
            file << attendence.second.get_id() << "," << attendence.second.get_student().get_id() << "," << attendence.second.get_course().get_id() << "," << attendence.second.get_date().toString() << "," << attendence.second.get_status() << endl;
        }
        file.close();
    }
}
void Attendence::refresh(map<int, Attendence>& attendences, const map<int, Student>& students, const map<int, Course>& courses) {
    write(attendences);
    attendences = read(students, courses);
}

Storage1<string> Attendence::get_headings() {
    return {"ID", "Student", "Course", "Date", "Status"};
}
Storage1<int> Attendence::get_widths() {
    return {5, 20, 20, 12, 8};
}
void Attendence::display_headings() {
    Utils::display_column(get_headings(), get_widths(), true);
}
void Attendence::display() const {
    Utils::display_column({to_string(id), student.get_name(), course.get_course_name(), date.toString(), string(1, status)}, get_widths(), false);
}
void Attendence::display_all(const map<int, Attendence>& attendences) {
    display_headings();
    for (auto& attendence : attendences) {
        attendence.second.display();
    }
    Utils::display_column_separator(get_widths());
}

bool Attendence::change_status() {
    char status = Utils::get_input("Enter new status: ", 1, true)[0];
    if (status == 'P' || status == 'A' || status == 'L') {
        set_status(status);
        return true;
    }
    return false;
}
bool Attendence::mark_attendence(map<int, Attendence>& attendences, const map<int, Student>& students, const map<int, Course>& courses) {
    if (courses.size() == 0) {
        Utils::print("No courses available.", INFO);
        _getch();
        return false;
    }

    map<int, string> selected_course;
    for (auto& course : courses) {
        selected_course[course.first] = course.second.get_course_name();
    }

    int course_id = Utils::option_selector<int>(selected_course, "Select a course to mark attendence for: ", 50);
    Storage1<Student> selected_students;
    for (auto& student : students) {
        if (student.second.find_enrolled_course(course_id) != -1) {
            selected_students.push_back(student.second);
        }
    }
    if (selected_students.empty()) {
        Utils::print("No students enrolled in the selected course.", INFO);
        _getch();
        return false;
    }
    Utils::print("Selected course: " + courses.at(course_id).get_course_name() + " (" + to_string(selected_students.size()) + " students)");
    Utils::print("Press 'P' for present, 'A' for absent, 'L' for leave.");
    for (int i = 0; i < selected_students.size(); i++) {
    again_label:
        string status = Utils::get_input("Enter status for " + selected_students[i].get_name() + ": ", 40);
        if (status != "P" && status != "A" && status != "L") {
            Utils::print("Invalid status.", INFO);
            _getch();
            Utils::remove_last_lines(2);
            goto again_label;
        }

        int attendence_id = attendences.empty() ? 1 : attendences.rbegin()->first + 1;
        Attendence attendence(attendence_id, selected_students[i], courses.at(course_id), Date(), status[0]);
        attendences[attendence_id] = attendence;
    }
    return true;
}
// ------------------------------- Ready for testing ------------------------------- //