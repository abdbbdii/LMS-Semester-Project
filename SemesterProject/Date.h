#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "Storage.h"
#include <ctime>
#include "Utils.h"
#include <map>

using namespace std;

class Date {
private:
    int day, month, year;

public:
    // Default constructor
    Date();

    // Parameterized constructor
    Date(const int& day, const int& month, const int& year);
    Date(const string& str);

    // Setters
    void set_day(const int& day);
    void set_month(const int& month);
    void set_year(const int& year);

    // Getters
    int get_day() const;
    int get_month() const;
    int get_year() const;

    string toString() const;
    bool ConstructFromStr(const string& str);
    int compare(const Date& date);
    bool operator<(const Date& date);
    bool operator>(const Date& date);
    bool operator==(const Date& date);
    bool operator<=(const Date& date);
    bool operator>=(const Date& date);
    bool operator!=(const Date& date);
    friend ostream& operator<<(ostream& out, const Date& date);
    friend istream& operator>>(istream& in, Date& date);
    static bool isValidDate(const int& day_, const int& month_, const int& year_);
    static bool isValidDate(string);
};
