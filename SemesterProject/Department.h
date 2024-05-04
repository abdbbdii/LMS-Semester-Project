#pragma once

#include "Faculty.h"
#include "Utils.h"
#include <fstream>
#include <string>
#include "Storage.h"
#include <map>

using namespace std;

class Department {
private:
    string name, alias;
    Faculty department_head;

public:
    // Default constructor
    Department();

    // Parameterized constructor
    Department(const string& alias, const Faculty& department_head, const string& name);
    Department(const string& alias);

    // Setters
    void set_alias(const string& alias);
    void set_department_head(const Faculty& department_head);
    void set_name(const string& name);

    // Getters
    string get_alias() const;
    Faculty get_department_head() const;
    string get_name() const;

    // Member functions
    void display() const;

    // Static member functions
    static map<string, Department> read();
    static void write(const map<string, Department>& departments);
    static void refresh(map<string, Department>& departments);
    static void display_headings();
    static Storage1<int> get_widths();
    static Storage1<string> get_headings();
    static void display_all(const map<string, Department>& departments);
    static bool register_department(map<string, Department>& departments, map<int, Faculty>& faculties);
};
// -------------------------------------------- Ready for testing -------------------------------------------- //
