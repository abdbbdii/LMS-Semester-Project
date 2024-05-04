#include "Club.h"

using namespace std;

// Default constructor
Club::Club() : club_id(-1), name(""), description(""), events({}), student_ids({}) {}

// Parameterized constructor
Club::Club(const int& club_id, const string& name, const string& description, const Storage1<Event>& events, const Storage1<int>& student_ids) : club_id(club_id), name(name), description(description), events(events), student_ids(student_ids) {}
Club::Club(const int& club_id) {
	map<int, Club> clubs = read({});
	*this = clubs[club_id];
}

// Setters
void Club::set_id(const int& club_id) { this->club_id = club_id; }
void Club::set_name(const string& name) { this->name = name; }
void Club::set_description(const string& description) { this->description = description; }
void Club::set_student_ids(const Storage1<int>& student_ids) { this->student_ids = student_ids; }
void Club::set_events(const Storage1<Event>& event_ids) { this->events = events; }

// Getters
int Club::get_id() const { return club_id; }
string Club::get_name() const { return name; }
string Club::get_description() const { return description; }
Storage1<int> Club::get_student_ids() const { return student_ids; }
Storage1<Event> Club::get_events() const { return events; }

// Member functions
int Club::find_member_id(const int& id) const {
	for (int i = 0; i < student_ids.size(); i++) {
		if (student_ids[i] == id) {
			return i;
		}
	}
	return -1;
}
int Club::find_event_id(const int& event_id) const {
	for (int i = 0; i < events.size(); i++) {
		if (events[i].get_event_id() == event_id) {
			return i;
		}
	}
	return -1;
}
bool Club::add_member_id(const int& id) {
	if (find_member_id(id) == -1) {
		student_ids.push_back(id);
		return true;
	}
	return false;
}
bool Club::add_event_id(const Event& event) {
	if (find_event_id(event.get_event_id()) == -1) {
		events.push_back(event);
		return true;
	}
	return false;
}
bool Club::remove_event_id(const int& event_id) {
	int index = find_event_id(event_id);
	if (index != -1) {
		events.erase(events.begin() + index);
		return true;
	}
	return false;
}
bool Club::remove_member_id(const int& id) {
	int index = find_member_id(id);
	if (index != -1) {
		student_ids.erase(student_ids.begin() + index);
		return true;
	}
	return false;
}
map<int, Club> Club::read(const map<int, Event>& events) {
	map<int, Club> clubs;
	ifstream file("data/clubs.csv");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			Storage1<string> tokens = Utils::split(line, ',');
			if (tokens.size() == 5) {
				Storage1<int> student_ids;
				Storage1<Event> club_events;

				Storage1<string> student_tokens = Utils::split(tokens[3], ';');
				if (student_tokens[0] != "")
					for (int i = 0; i < student_tokens.size(); i++)
						student_ids.push_back(stoi(student_tokens[i]));

				Storage1<string> event_tokens = Utils::split(tokens[4], ';');
				if (event_tokens[0] != "")
					for (int i = 0; i < event_tokens.size(); i++)
						club_events.push_back(events.at(stoi(event_tokens[i])));

				clubs[stoi(tokens[0])] = Club(stoi(tokens[0]), tokens[1], tokens[2], club_events, student_ids);

			}
		}
		file.close();
	}
	return clubs;
}
void Club::write(const map<int, Club>& clubs) {
	ofstream file("data/clubs.csv");
	if (file.is_open()) {
		for (auto& club : clubs) {
			file << club.second.get_id() << "," << club.second.get_name() << "," << club.second.get_description() << ",";
			for (int i = 0; i < club.second.get_student_ids().size(); i++) {
				file << club.second.get_student_ids()[i];
				if (i != club.second.get_student_ids().size() - 1) {
					file << ";";
				}
			}
			file << ",";
			for (int i = 0; i < club.second.get_events().size(); i++) {
				file << club.second.get_events()[i].get_event_id();
				if (i != club.second.get_events().size() - 1) {
					file << ";";
				}
			}
			file << endl;
		}
		file.close();
	}
}
void Club::refresh(map<int, Club>& clubs, const map<int, Event>& events) {
	write(clubs);
	clubs = read(events);
}

Storage1<string> Club::get_headings() {
	return { "ID", "Club Name", "Club Description", "Students", "Events" };
}
Storage1<int> Club::get_widths() {
	return { 5, 30, 80, 10, 10 };
}
void Club::display_headings() {
	Utils::display_column({ "ID", "Club Name", "Club Description", "Students", "Events" }, get_widths(), true);
}
void Club::display() const {
	Utils::display_column({ to_string(club_id), name, description, to_string(student_ids.size()), to_string(events.size()) }, get_widths());
}
void Club::display_all(const map<int, Club>& clubs) {
	display_headings();
	for (auto& club : clubs) {
		club.second.display();
	}
	Utils::display_column_separator(get_widths());
}
bool Club::create_club(map<int, Club>& clubs) {
	Club club;
	club.set_id(clubs.empty() ? 1 : clubs.rbegin()->first + 1);
	club.set_name(Utils::get_input("Enter Club Name", 30));
	club.set_description(Utils::get_input("Enter Club Description", 30));
	clubs[club.get_id()] = club;
	return true;
}
// ------------------------------------- Ready for testing ------------------------------------- //