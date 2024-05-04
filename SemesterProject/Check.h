#pragma once

#include <regex>
#include <map>
#include "Storage.h"

using namespace std;

class Check {
public:
	map<string, string> patterns = {
		{"email", "^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+(\\.[a-zA-Z0-9-]+)+$"},
		{"password", "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?!.*\\,).{8,}$"},
		{"phone_number", "^\\+?[0-9]{10,13}$"},
		{"roll_number", "^[0-9]{4}-[A-Z]{2,4}-[0-9]{1,3}$"},
		{"date", "^\\d{1,2}-\\d{1,2}-\\d{4}$"},
		{"name", "^[a-zA-Z ]+$"},
		{"double", "^\\d+(\\.\\d+)?$"},
		{"int", "^\\d+$"},
		{"char", "^.$"},
		{"string", "^.*$"},
		{"bool", "^(true|false)$"},
		{"semester", "^Summer|Spring|Fall$"}
	};

	static bool is_valid(const string& value, const string& pattern, const int& max_length = 0) {
		if (max_length > 0 && value.length() > max_length) {
			return false;
		}
		regex regex(pattern);
		return !regex_match(value, regex);
	}
};