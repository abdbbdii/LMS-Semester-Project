#include "Assignment.h"

using namespace std;

// Default constructor
Assignment::Assignment() : id(-1), due_date(Date()), description(""), course(Course()) {}

// Parameterized constructor
Assignment::Assignment(const int& id, const Date& due_date, const string& description, const Course& course) : id(id), due_date(due_date), description(description), course(course) {}
Assignment::Assignment(const int& id) {
    map<int, Assignment> assignments = read({});
    *this = assignments[id];
}

// Setters
void Assignment::set_id(const int& id) { this->id = id; }
void Assignment::set_due_date(const Date& due_date) { this->due_date = due_date; }
void Assignment::set_description(const string& description) { this->description = description; }
void Assignment::set_course(const Course& course) { this->course = course; }

// Getters
int Assignment::get_id() const { return id; }
string Assignment::get_description() const { return description; }
Date Assignment::get_due_date() const { return due_date; }
Course Assignment::get_course() const { return course; }

// Member functions
map<int, Assignment> Assignment::read(const map<int, Course>& courses) {
    map<int, Assignment> assignments;
    ifstream file("data/assignments.csv");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Storage1<string> tokens = Utils::split(line, ',');
            if (tokens.size() == 4) {
                Assignment assignment(stoi(tokens[0]), Date(tokens[1]), tokens[2], courses.at(stoi(tokens[3])));
                assignments[assignment.get_id()] = assignment;
            }
        }
        file.close();
    }
    return assignments;
}
void Assignment::write(const map<int, Assignment>& assignments) {
    ofstream file("data/assignments.csv");
    if (file.is_open()) {
        for (auto& assignment : assignments) {
            file << assignment.second.get_id() << "," << assignment.second.get_due_date().toString() << "," << assignment.second.get_description() << "," << assignment.second.get_course().get_id() << endl;
        }
        file.close();
    }
}
void Assignment::refresh(map<int, Assignment>& assignments, const map<int, Course>& courses) {
    write(assignments);
    assignments = read(courses);
}

Storage1<string> Assignment::get_headings() {
    return {"ID", "Description", "Course Name", "Due Date"};
}
Storage1<int> Assignment::get_widths() {
    return {5, 50, 40, 12};
}
void Assignment::display_headings() {
    Utils::display_column(get_headings(), get_widths(), true);
}
void Assignment::display() const {
    Utils::display_column({to_string(id), description, course.get_course_name(), due_date.toString()}, get_widths());
}
void Assignment::display_all(const map<int, Assignment>& assignments) {
    display_headings();
    for (auto& assignment : assignments) {
        assignment.second.display();
    }
    Utils::display_column_separator(get_widths());
}

bool Assignment::create_assignment(map<int, Assignment>& assignments, const map<int, Course>& courses) {
    if (courses.empty()) {
        Utils::print("No courses available. Please add courses first.", WARNING);
        _getch();
        return false;
    }

    map<int, string> course_names;
    for (auto& course : courses) {
        course_names[course.first] = course.second.get_course_name();
    }

    int selected_course_id = Utils::option_selector<int>(course_names, "Select a course: ", 50);
    Utils::print("Creating Assignment for " + course_names[selected_course_id], BOLD);
    cout << endl;
    Assignment assignment;
    assignment.set_id(assignments.empty() ? 1 : assignments.rbegin()->first + 1);
    assignment.set_description(Utils::get_input("Enter description: ", 30));
    string date = Utils::get_input("Enter due date (dd-mm-yyyy): ", 30);
    if (!Check::is_valid(date, "date")) {
        Utils::print("Invalid date format.", INFO);
        _getch();
        return false;
    }
    if (!Date::isValidDate(date)) {
        Utils::print("Invalid date.", INFO);
        _getch();
        return false;
    }
    if (Date(date) < Date()) {
        Utils::print("Due date cannot be in the past.", INFO);
        _getch();
        return false;
    }
    assignment.set_due_date(Date(date));
    assignment.set_course(courses.at(selected_course_id));
    assignments[assignment.get_id()] = assignment;
}
bool Assignment::change_due_date() {
    string date = Utils::get_input("Enter new due date (dd-mm-yyyy): ", 30);
    if (!Check::is_valid(date, "date")) {
        Utils::print("Invalid date format.", INFO);
        _getch();
        return false;
    }
    if (!Date::isValidDate(date)) {
        Utils::print("Invalid date.", INFO);
        _getch();
        return false;
    }
    if (Date(date) < Date()) {
        Utils::print("Due date cannot be in the past.", INFO);
        _getch();
        return false;
    }
    set_due_date(Date(date));
    return true;
}
bool Assignment::change_description() {
    set_description(Utils::get_input("Enter new description: ", 30));
    return true;
}
bool Assignment::change_course(const map<int, Course>& courses) {
    map<int, string> course_names;
    for (auto& course : courses) {
        course_names[course.first] = course.second.get_course_name();
    }
    int selected_course_id = Utils::option_selector<int>(course_names, "Select a course: ", 50);
    set_course(courses.at(selected_course_id));
    return true;
}
bool Assignment::edit_assignment(map<int, Assignment>& assignments, const map<int, Course>& courses) {
    if (assignments.empty()) {
        Utils::print("No assignments available. Please add assignments first.", WARNING);
        _getch();
        return false;
    }
    map<int, string> assignment_names;
    for (auto& assignment : assignments) {
        for (auto& course : courses) {
			if (assignment.second.get_course().get_id() == course.first) {
				assignment_names[assignment.first] = assignment.second.get_description() + " (" + course.second.get_course_name() + ")";
			}
		}
    }
    int selected_assignment_id = Utils::option_selector<int>(assignment_names, "Select an assignment: ", 80);
    Utils::print("Editing Assignment: " + assignment_names[selected_assignment_id], BOLD);
    cout << endl;
    Assignment& assignment = assignments[selected_assignment_id];
edit_assignment_label:
    switch (Utils::create_menu({"Change due date", "Change description", "Change course"}, false, true, 40)) {
    case -1:
        return true;
    case 1:
        if (assignment.change_due_date()) {
            Utils::print("Due date changed successfully.", SUCCESS);
            _getch();
        }
        break;
    case 2:
        if (assignment.change_description()) {
            Utils::print("Description changed successfully.", SUCCESS);
            _getch();
        }
        break;
    case 3:
        if (assignment.change_course(courses)) {
            Utils::print("Course changed successfully.", SUCCESS);
            _getch();
        }
        break;
    }
    goto edit_assignment_label;
}
// -------------------------------------------- Ready for testing -------------------------------------------- //