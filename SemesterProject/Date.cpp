#include "Date.h"

// Default constructor
Date::Date() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    day = ltm.tm_mday;
    month = 1 + ltm.tm_mon;
    year = 1900 + ltm.tm_year;
}

// Parameterized constructor
Date::Date(const int& day, const int& month, const int& year) : day(day), month(month), year(year) {
    if (!isValidDate(day, month, year)) {
        Utils::print("Invalid date", ERROR);
    }
}

// String constructor
Date::Date(const string& str) {
    if (!ConstructFromStr(str)) {
        Utils::print("Invalid date", ERROR);
    }
}

// Setter methods
void Date::set_day(const int& day) { this->day = day; }
void Date::set_month(const int& month) { this->month = month; }
void Date::set_year(const int& year) { this->year = year; }

// Getter methods
int Date::get_day() const { return day; }
int Date::get_month() const { return month; }
int Date::get_year() const { return year; }

// Other methods
bool Date::isValidDate(const int& day_, const int& month_, const int& year_) {
    if (year_ < 1900 || year_ > 2100)
        return false;
    if (month_ < 1 || month_ > 12)
        return false;
    switch (month_) {
    case 2: // February
        if ((year_ % 4 == 0 && year_ % 100 != 0) || (year_ % 400 == 0))
            return day_ >= 1 && day_ <= 29;
        else
            return day_ >= 1 && day_ <= 28;
    case 4:  // April
    case 6:  // June
    case 9:  // September
    case 11: // November
        return day_ >= 1 && day_ <= 30;
    default:
        return day_ >= 1 && day_ <= 31;
    }
}
bool Date::isValidDate(string str) {
    stringstream ss(str);
    Storage1<int> v;
    string token;
    while (getline(ss, token, '-')) {
        v.push_back(stoi(token));
    }
    if (v.size() != 3) {
        return false;
    } else {
        return isValidDate(v[0], v[1], v[2]);
    }
}

// String representation of the date
string Date::toString() const {
    return to_string(day) + "-" + to_string(month) + "-" + to_string(year);
}

ostream& operator<<(ostream& out, const Date& date) {
    out << date.toString();
    return out;
}

bool Date::ConstructFromStr(const string& str) {
    stringstream ss(str);
    Storage1<int> v;
    string token;
    while (getline(ss, token, '-')) {
        v.push_back(stoi(token));
    }
    if (v.size() != 3) {
        return false;
    } else if (isValidDate(v[0], v[1], v[2])) {
        day = v[0];
        month = v[1];
        year = v[2];
        return true;
    } else {
        return false;
    }
}

istream& operator>>(istream& in, Date& date) {
    string str;
    in >> str;
    if (!date.ConstructFromStr(str))
        Utils::print("Invalid date", ERROR);
    return in;
}

int Date::compare(const Date& date) {
    if (year != date.year)
        return year - date.year;
    if (month != date.month)
        return month - date.month;
    return day - date.day;
}

bool Date::operator<(const Date& date) {
    return compare(date) < 0;
}

bool Date::operator>(const Date& date) {
    return compare(date) > 0;
}

bool Date::operator==(const Date& date) {
    return compare(date) == 0;
}

bool Date::operator<=(const Date& date) {
    return compare(date) <= 0;
}

bool Date::operator>=(const Date& date) {
    return compare(date) >= 0;
}

bool Date::operator!=(const Date& date) {
    return compare(date) != 0;
}
