#include "Department.h"

using namespace std;

// Default constructor
Department::Department() : alias(""), department_head(Faculty()), name("") {}

// Parameterized constructor
Department::Department(const string& alias, const Faculty& department_head, const string& name) : alias(alias), department_head(department_head), name(name) {}
Department::Department(const string& alias) {
	map<string, Department> departments = read();
	*this = departments[alias];
}

// Setters
void Department::set_alias(const string& alias) { this->alias = alias; }
void Department::set_department_head(const Faculty& department_head) { this->department_head = department_head; }
void Department::set_name(const string& name) { this->name = name; }

// Getters
string Department::get_alias() const { return alias; }
Faculty Department::get_department_head() const { return department_head; }
string Department::get_name() const { return name; }

// Member functions
map<string, Department> Department::read() {
	map<string, Department> departments;
	ifstream file("data/departments.csv");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			Department department(tokens[0], Faculty(stoi(tokens[1])), tokens[2]);
			departments[department.get_alias()] = department;
		}
		file.close();
	}
	return departments;
}
void Department::write(const map<string, Department>& departments) {
	ofstream file("data/departments.csv");
	if (file.is_open()) {
		for (auto& department : departments) {
			file << department.second.get_alias() << "," << department.second.get_department_head().get_id() << "," << department.second.get_name() << endl;
		}
		file.close();
	}
}
void Department::refresh(map<string, Department>& departments) {
	write(departments);
	departments = read();
}

Storage1<int> Department::get_widths() {
	return { 5, 30, 20 };
}
Storage1<string> Department::get_headings() {
	return { "Alias", "Department Name", "Head Name" };
}
void Department::display_headings() {
	Utils::display_column(get_headings(), get_widths(), true);
}
void Department::display() const {
	Utils::display_column({ alias, name, department_head.get_name() }, get_widths());
}
void Department::display_all(const map<string, Department>& departments) {
	display_headings();
	for (auto& department : departments) {
		department.second.display();
	}
	Utils::display_column_separator(get_widths());
}
bool Department::register_department(map<string, Department>& departments, map<int, Faculty>& faculties) {
	Department department;

alias_again:
	string alias = Utils::get_input("Enter department alias: ", 30);
	if (departments.find(alias) != departments.end()) {
		Utils::print("Department with alias " + department.get_alias() + " already exists.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto alias_again;
	}
	department.set_alias(alias);

	string name = Utils::get_input("Enter department name: ", 30);
	if (!Check::is_valid(name, "name")) {
		Utils::print("Invalid department name.", ERROR);
		_getch();
		Utils::remove_last_lines(2);
		goto alias_again;
	}
	department.set_name(name);

	map <int, string> faculty_names;
	for (auto& faculty : faculties) {
		faculty_names[faculty.first] = faculty.second.get_name();
	}
	int faculty_id = Utils::option_selector(faculty_names, "Select department head: ", 40);
	department.set_department_head(faculties[faculty_id]);

	if (faculties[faculty_id].get_auth_level() < 3) {
		faculties[faculty_id].set_auth_level(3);
	}

	departments[department.get_alias()] = department;
	return true;
}
// -------------------------------------------- Ready for testing -------------------------------------------- //