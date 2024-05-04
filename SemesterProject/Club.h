#pragma once

#include "Utils.h"
#include <fstream>
#include <map>
#include <string>
#include "Storage.h"
#include "Event.h"

using namespace std;

class Club {
private:
	int club_id;
	string name, description;
	Storage1<int> student_ids;
	Storage1<Event> events;

public:
	// Default constructor
	Club();

	// Parameterized constructor
	Club(const int& id, const string& name, const string& description, const Storage1<Event>& events = {}, const Storage1<int>& student_ids = {});
	Club(const int& id);

	// Setters
	void set_id(const int& club_id);
	void set_name(const string& name);
	void set_description(const string& description);
	void set_student_ids(const Storage1<int>& student_ids);
	void set_events(const Storage1<Event>& events);

	// Getters
	int get_id() const;
	string get_name() const;
	string get_description() const;
	Storage1<Event> get_events() const;
	Storage1<int> get_student_ids() const;

	// Member functions
	int find_member_id(const int& id) const;
	int find_event_id(const int& event_id) const;
	bool add_member_id(const int& id);
	bool add_event_id(const Event& event);
	bool remove_member_id(const int& id);
	bool remove_event_id(const int& event_id);
	void display() const;

	// Static member functions
	static map<int, Club> read(const map<int, Event>& events);
	static void write(const map<int, Club>& clubs);
	static void refresh(map<int, Club>& clubs, const map<int, Event>& events);
	static void display_headings();
	static Storage1<string> get_headings();
	static Storage1<int> get_widths();
	static void display_all(const map<int, Club>& clubs);
	static bool create_club(map<int, Club>& clubs);
};
// ------------------------------------- Ready for testing ------------------------------------- //
