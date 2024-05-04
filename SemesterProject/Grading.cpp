#include "Grading.h"

// Default constructor
Grading::Grading() : id(-1), student(), course(), marks(0.0) {}

// Parameterized constructor
Grading::Grading(const int& id, const Student& student, const Course& course, double marks) : id(id), student(student), course(course), marks(marks) {}

// Setters
void Grading::set_grading_id(const int& id) { this->id = id; }
void Grading::set_student(const Student& student) { this->student = student; }
void Grading::set_course(const Course& course) { this->course = course; }
void Grading::set_marks(double marks) { this->marks = marks; }

// Getters
int Grading::get_id() const { return id; }
Student Grading::get_student() const { return student; }
Course Grading::get_course() const { return course; }
double Grading::get_marks() const { return marks; }

// Member functions
map<int, Grading> Grading::read(const map<int, Student>& students, const map<int, Course>& courses) {
    map<int, Grading> gradings;
    ifstream file("data/gradings.csv");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Storage1<string> tokens = Utils::split(line, ',');
            Grading grading(stoi(tokens[0]), students.at(stoi(tokens[1])), courses.at(stoi(tokens[2])), stod(tokens[3]));
            gradings[grading.get_id()] = grading;
        }
        file.close();
    }
    return gradings;
}
void Grading::write(const map<int, Grading>& gradings) {
    ofstream file("data/gradings.csv");
    if (file.is_open()) {
        for (auto& grading : gradings) {
            file << grading.second.get_id() << "," << grading.second.get_student().get_id() << "," << grading.second.get_course().get_id() << "," << grading.second.get_marks() << endl;
        }
        file.close();
    }
}
void Grading::refresh(map<int, Grading>& gradings, const map<int, Student>& students, const map<int, Course>& courses) {
    write(gradings);
    gradings = read(students, courses);
}

Storage1<int> Grading::get_widths() {
    return {5, 20, 20, 10};
}
Storage1<string> Grading::get_headings() {
    return {"ID", "Student Name", "CourseName", "Marks"};
}
void Grading::display_headings() {
    Utils::display_column(get_headings(), get_widths(), true);
}
void Grading::display() const {
    Utils::display_column({to_string(id), student.get_name(), course.get_course_name(), to_string(marks)}, get_widths());
}
void Grading::display_all(const map<int, Grading>& gradings) {
    display_headings();
    for (auto& grading : gradings) {
        grading.second.display();
    }
    Utils::display_column_separator(get_widths());
}

bool Grading::grade_students(map<int, Grading>& gradings, const map<int, Student>& students, const map<int, Course>& courses) {
    if (courses.size() == 0) {
        Utils::print("No courses available.", INFO);
        _getch();
        return false;
    }

    map<int, string> selected_course;
    for (auto& course : courses) {
        selected_course[course.first] = course.second.get_course_name();
    }

    int course_id = Utils::option_selector<int>(selected_course, "Select a course to grade: ", 50);
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
    for (int i = 0; i < selected_students.size(); i++) {
    again_label:
        string marks = Utils::get_input("Enter marks for " + selected_students[i].get_name() + ": ", 40);
        if (!Check::is_valid(marks, "double")) {
            Utils::print("Invalid marks. Please enter a valid number.", ERROR);
            goto again_label;
        }
        Grading grading(gradings.size() + 1, selected_students[i], courses.at(course_id), stod(marks));
        gradings[grading.get_id()] = grading;
    }
    return true;
}
// --------------------------- Ready for testing --------------------------- //