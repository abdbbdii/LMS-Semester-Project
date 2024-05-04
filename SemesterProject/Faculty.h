#pragma once

#include "Check.h"
#include "Utils.h"
#include <fstream>
#include <string>
#include "Storage.h"
#include <map>

using namespace std;

class Faculty {
private:
    int id, auth_level;
    string name, email, phone_number, password;

public:
    // Default constructor
    Faculty();

    // Parameterized constructor
    Faculty(const int& id, const string& name, const string& email, const string& password, const string& phone_number, const int& auth_level);
    Faculty(const int& id);

    // Setters
    void set_id(const int& id);
    void set_name(const string& name);
    void set_email(const string& email);
    void set_password(const string& password);
    void set_phone_number(const string& phone_number);
    void set_auth_level(const int& auth_level);

    // Getters
    int get_id() const;
    string get_name() const;
    string get_email() const;
    string get_password() const;
    string get_phone_number() const;
    int get_auth_level() const;

    // Member functions
    void display_faculty() const;
    bool change_faculty_phone_number();
    bool change_faculty_password();
    bool change_faculty_auth_level(map<int, Faculty>& faculties);

    // Static member functions
    static map<int, Faculty> read();
    static void write(const map<int, Faculty>& faculties);
    static void refresh(map<int, Faculty>& faculties);
    static void display_headings();
    static Storage1<string> get_headings();
    static Storage1<int> get_widths();
    static void display_all(const map<int, Faculty>& faculties);
    static Faculty login(const map<int, Faculty>& faculties);
    static map<int, string> get_faculty_auth_levels();
    static bool admit_faculty(map<int, Faculty>& faculties);
};
// ----------------------------------- Ready for testing -------------------------------------- //
