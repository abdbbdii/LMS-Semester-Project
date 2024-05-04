#pragma once

#include "Date.h"
#include "Utils.h"
#include <fstream>
#include "Storage.h"
#include <map>

using namespace std;

class Event {
private:
	int event_id;
	string title, description;
	Date date;
	string venue;
	Storage1<int> attendee_ids;

public:
	// Default constructor
	Event();

	// Parameterized constructor
	Event(const int& event_id, const string& title, const string& description, const Date& date, const string& venue, const Storage1<int> attendee_ids = {});
	Event(const int& event_id);

	// Setters
	void set_event_id(const int& event_id);
	void set_title(const string& title);
	void set_description(const string& description);
	void set_date(const Date& date);
	void set_venue(const string& venue);
	void set_attendee_ids(const Storage1<int> attendee_ids);

	// Getters
	int get_event_id() const;
	string get_title() const;
	string get_description() const;
	Date get_date() const;
	string get_venue() const;
	Storage1<int> get_attendee_ids() const;

	// Member functions
	int find_attendee(const int& id);
	bool add_attendee(const int& id);
	bool remove_attendee(const int& id);
	void display_event() const;

	// Static member functions
	static map<int, Event> read();
	static void write(const map<int, Event>& events);
	static void refresh(map<int, Event>& events);
	static void display_headings();
	static Storage1<string> get_headings();
	static Storage1<int> get_widths();
	static void display_events(const map<int, Event>& events);
};
// ----------------------------------------- Ready for testing ----------------------------------------- //
