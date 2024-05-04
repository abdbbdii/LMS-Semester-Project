#include "Faculty.h"

using namespace std;

// Default constructor
Faculty::Faculty() : id(-1), name(""), email(""), phone_number(""), password(""), auth_level(0) {}

// Parameterized constructor
Faculty::Faculty(const int& id, const string& name, const string& email, const string& password, const string& phone_number, const int& auth_level) : id(id), name(name), email(email), password(password), phone_number(phone_number), auth_level(auth_level) {}
Faculty::Faculty(const int& id) {
	map<int, Faculty> faculties = read();
	*this = faculties[id];
}

// Setters
void Faculty::set_id(const int& id) { this->id = id; }
void Faculty::set_name(const string& name) { this->name = name; }
void Faculty::set_email(const string& email) { this->email = email; }
void Faculty::set_password(const string& password) { this->password = password; }
void Faculty::set_phone_number(const string& phone_number) { this->phone_number = phone_number; }
void Faculty::set_auth_level(const int& auth_level) { this->auth_level = auth_level; }

// Getters
int Faculty::get_id() const { return id; }
string Faculty::get_name() const { return name; }
string Faculty::get_email() const { return email; }
string Faculty::get_password() const { return password; }
string Faculty::get_phone_number() const { return phone_number; }
int Faculty::get_auth_level() const { return auth_level; }

// Member fuctions
map<int, Faculty> Faculty::read() {
	map<int, Faculty> faculties;
	faculties.clear();
	ifstream file("data/faculties.csv");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			if (tokens.size() == 6) {
				Faculty faculty(stoi(tokens[0]), tokens[1], tokens[2], tokens[3], tokens[4], stoi(tokens[5]));
				faculties[faculty.get_id()] = faculty;
			}
		}
		file.close();
	}
	return faculties;
}
void Faculty::write(const map<int, Faculty>& faculties) {
	ofstream file("data/faculties.csv");
	if (file.is_open()) {
		for (auto& faculty : faculties) {
			file << faculty.second.get_id() << "," << faculty.second.get_name() << "," << faculty.second.get_email() << "," << faculty.second.get_password() << "," << faculty.second.get_phone_number() << "," << faculty.second.get_auth_level() << endl;
		}
		file.close();
	}
}
Faculty Faculty::login(const map<int, Faculty>& faculties) {
	string email = Utils::get_input("Enter email: ");
	string password = Utils::get_input("Enter password: ", 20, false, true);
	for (auto& faculty : faculties) {
		if (faculty.second.get_email() == email && faculty.second.get_password() == password) {
			return faculty.second;
		}
	}
	return Faculty();
}

Storage1<string> Faculty::get_headings() {
	return { "ID", "Name", "Email", "Phone Number", "Authority Level" };
}
Storage1<int> Faculty::get_widths() {
	return { 5, 20, 30, 15, 20 };
}
void Faculty::display_headings() {
	Utils::display_column(get_headings(), get_widths(), true);
}
void Faculty::display_faculty() const {
	Utils::display_column({ to_string(id), name, email, phone_number, to_string(auth_level) }, get_widths());
}
void Faculty::display_all(const map<int, Faculty>& faculties) {
	display_headings();
	for (auto& faculty : faculties) {
		faculty.second.display_faculty();
	}
	Utils::display_column_separator(get_widths());
}
void Faculty::refresh(map<int, Faculty>& faculties) {
	write(faculties);
	faculties = read();
}
bool Faculty::change_faculty_phone_number() {
	phone_number = Utils::get_input("Enter new phone number: ", 30);
	if (Check::is_valid(phone_number, "phone_number")) {
		set_phone_number(phone_number);
		return true;
	}
	Utils::print("Invalid phone number.", INFO);
	_getch();
	return false;
}
bool Faculty::change_faculty_password() {
	string old_password = Utils::get_input("Enter old password: ", 30, false, true);
	if (old_password != password) {
		Utils::print("Invalid password.", INFO);
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
map<int, string> Faculty::get_faculty_auth_levels() {
	return {
		{1, "Staff"},
		{2, "Teacher"},
		{3, "Dean"},
		{4, "Admin"},
	};
}
bool Faculty::change_faculty_auth_level(map<int, Faculty>& faculties) {
	map<int, string> faculty_names;
	for (auto& faculty : faculties) {
		if (faculty.second.get_auth_level() < get_auth_level()) {
			faculty_names[faculty.second.get_id()] = faculty.second.get_name();
		}
	}

	if (faculty_names.size() == 0) {
		Utils::print("No faculties available to change authority level.", INFO);
		_getch();
		return false;
	}

	map<int, string> available_auth_levels;
	for (auto& auth_level : get_faculty_auth_levels()) {
		if (auth_level.first <= get_auth_level()) {
			available_auth_levels[auth_level.first] = auth_level.second;
		}
	}

	int faculty_id = Utils::option_selector(faculty_names, "Select faculty: ", 50);
	int auth_level = Utils::option_selector(available_auth_levels, "Select new authority level: ", 50);

	faculties[faculty_id].set_auth_level(auth_level);
	return true;
}
bool Faculty::admit_faculty(map<int, Faculty>& faculties) {
	Faculty faculty;
	faculty.set_id(faculties.size() == 0 ? 1 : faculties.rbegin()->first + 1);

name_again:
	string name = Utils::get_input("Enter name: ", 30);
	if (!Check::is_valid(name, "name")) {
		Utils::print("Name must be at least 3 characters long and should have valid characters.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto name_again;
	}
	faculty.set_name(name);

email_again:
	string email = Utils::get_input("Enter email: ", 30);
	if (!Check::is_valid(email, "email")) {
		Utils::print("Invalid email.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto email_again;
	}
	faculty.set_email(email);

password_again:
	string password = Utils::get_input("Enter name: ", 30);
	if (!Check::is_valid(password, "password")) {
		Utils::print("Password must be at least 8 characters long and should have valid characters.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto password_again;
	}
	faculty.set_password(password);

phone_again:
	string phone_number = Utils::get_input("Enter phone number: ", 30);
	if (!Check::is_valid(phone_number, "phone_number")) {
		Utils::print("Invalid phone number.", INFO);
		_getch();
		Utils::remove_last_lines(2);
		goto phone_again;
	}
	faculty.set_phone_number(phone_number);

	int auth_level = Utils::option_selector(get_faculty_auth_levels(), "Select authority level: ", 40);
	faculty.set_auth_level(auth_level);

	faculties[faculty.get_id()] = faculty;
	return true;
}
// ------------------------------------------ Ready for testing ----------------------------------------- //