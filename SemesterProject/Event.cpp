#include "Event.h"

using namespace std;

// Default constructor
Event::Event() : event_id(-1), title(""), description(""), date(Date()), venue(""), attendee_ids({}) {}

// Parameterized constructor
Event::Event(const int& event_id, const string& title, const string& description, const Date& date, const string& venue, const Storage1<int> attendee_ids) : event_id(event_id), title(title), description(description), date(date), venue(venue), attendee_ids(attendee_ids) {}
Event::Event(const int& event_id) {
	map<int, Event> events = read();
	*this = events[event_id];
}

// Setters
void Event::set_event_id(const int& event_id) { this->event_id = event_id; }
void Event::set_title(const string& title) { this->title = title; }
void Event::set_description(const string& description) { this->description = description; }
void Event::set_date(const Date& date) { this->date = date; }
void Event::set_venue(const string& venue) { this->venue = venue; }
void Event::set_attendee_ids(const Storage1<int> attendee_ids) { this->attendee_ids = attendee_ids; }

// Getters
int Event::get_event_id() const { return event_id; }
string Event::get_title() const { return title; }
string Event::get_description() const { return description; }
Date Event::get_date() const { return date; }
string Event::get_venue() const { return venue; }
Storage1<int> Event::get_attendee_ids() const { return attendee_ids; }

// Member functions
int Event::find_attendee(const int& id) {
	for (int i = 0; i < attendee_ids.size(); i++) {
		if (attendee_ids[i] == id) {
			return i;
		}
	}
	return -1;
}
bool Event::add_attendee(const int& id) {
	if (find_attendee(id) == -1) {
		attendee_ids.push_back(id);
		return true;
	}
	return false;
}
bool Event::remove_attendee(const int& id) {
	int index = find_attendee(id);
	if (index != -1) {
		attendee_ids.erase(attendee_ids.begin() + index);
		return true;
	}
	return false;
}

map<int, Event> Event::read() {
	map<int, Event> events;
	ifstream file("data/events.csv");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			if (tokens.size() != 6) {
				continue;
			}
			Storage1<string> attendee_ids = Utils::split(tokens[5], ';');
			Storage1<int> attendees;
			for (int i = 0; i < attendee_ids.size(); i++) {
				attendees.push_back(stoi(attendee_ids[i]));
			}
			Event event(stoi(tokens[0]), tokens[1], tokens[2], Date(tokens[3]), tokens[4], attendees);
			events[event.get_event_id()] = event;
		}
		file.close();
	}
	return events;
}
void Event::write(const map<int, Event>& events) {
	ofstream file("data/events.csv");
	if (file.is_open()) {
		for (auto& event : events) {
			file << event.second.get_event_id() << ',' << event.second.get_title() << ',' << event.second.get_description() << ',' << event.second.get_date().toString() << ',' << event.second.get_venue() << ',';
			for (int i = 0; i < event.second.get_attendee_ids().size(); i++) {
				file << event.second.get_attendee_ids()[i];
				if (i < event.second.get_attendee_ids().size() - 1) {
					file << ';';
				}
			}
			file << endl;
		}
		file.close();
	}
}
void Event::refresh(map<int, Event>& events) {
	write(events);
	events = read();
}

Storage1<string> Event::get_headings() {
	return { "ID", "Title", "Description", "Date", "Venue", "Attendees" };
}
Storage1<int> Event::get_widths() {
	return { 5, 30, 80, 10, 20, 12 };
}
void Event::display_headings() {
	Utils::display_column(get_headings(), get_widths(), true);
}
void Event::display_event() const {
	Utils::display_column({ to_string(event_id), title, description, date.toString(), venue, to_string(attendee_ids.size()) }, get_widths());
}
void Event::display_events(const map<int, Event>& events) {
	display_headings();
	for (auto& event : events) {
		event.second.display_event();
	}
	Utils::display_column_separator(get_widths());
}
// ------------------------------------ Ready for testing ----------------------------------- //