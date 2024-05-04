#include <iostream>

#include <string>
#include "Storage.h"
#include "Assignment.h"
#include "Attendence.h"
#include "Club.h"
#include "Course.h"
#include "CourseFeedback.h"
#include "Date.h"
#include "Department.h"
#include "Event.h"
#include "Faculty.h"
#include "FacultyCourseAllocation.h"
#include "Student.h"
#include "Utils.h"
#include "Grading.h"
#include <conio.h>

// #include "StudentAssignmentSubmission.h" // maybe?
// #include "StudentClubEnrollment.h"       // maybe?
// #include "StudentCourseEnrollment.h"     // maybe?
// #include "StudentCourseFeedback.h"       // maybe?
// #include "StudentDepartmentFeedback.h"   // maybe?
// #include "StudentEventEnrollment.h"      // maybe?
// #include "StudentFacultyFeedback.h"      // maybe?

using namespace std;

int main() {
	Utils::print("Loading faculties...", INFO);
	map<int, Faculty> faculties = Faculty::read();
	Utils::print("Loading departments...", INFO);
	map<string, Department> departments = Department::read();
	Utils::print("Loading courses...", INFO);
	map<int, Course> courses = Course::read();
	Utils::print("Loading faculty_course_allocations...", INFO);
	map<int, FacultyCourseAllocation> faculty_course_allocations = FacultyCourseAllocation::read(faculties, courses);
	Utils::print("Loading assignments...", INFO);
	map<int, Assignment> assignments = Assignment::read(courses);
	Utils::print("Loading events...", INFO);
	map<int, Event> events = Event::read();
	Utils::print("Loading clubs...", INFO);
	map<int, Club> clubs = Club::read(events);
	Utils::print("Loading students...", INFO);
	map<int, Student> students = Student::read(courses, assignments, clubs);
	Utils::print("Loading course_feedbacks...", INFO);
	map<int, CourseFeedback> course_feedbacks = CourseFeedback::read(students, courses);
	Utils::print("Loading attendences...", INFO);
	map<int, Attendence> attendences = Attendence::read(students, courses);
	Utils::print("Loading grades...", INFO);
	map<int, Grading> grades = Grading::read(students, courses);

	// -------------------------------------------------------- //

again:
	system("cls");

	Faculty current_faculty;
	Student current_student;
	bool is_faculty = false;

	switch (Utils::create_menu({ "University of Technology and Engineering", "Login as Faculty", "Login as Student", "Exit" }, true, false, 42)) {
	case 1:
	login_facalty_label:
		Utils::print("Faculty Login\n", BOLD);
		current_faculty = Faculty::login(faculties);
		if (current_faculty.get_id() == -1) {
			system("cls");
			Utils::print("Login failed", ERROR);
			_getch();
			system("cls");
			switch (Utils::create_menu({ "Try again", "Login as Student", "Exit" })) {
			case 0:
				system("cls");
				goto login_facalty_label;
			case 1:
				system("cls");
				goto login_student_label;
			case 2:
				return 0;
			} // faculty login again switch
		}     // faculty login unsuccessful
		is_faculty = true;
		break;
	case 2:
	login_student_label:
		Utils::print("Student Login\n", BOLD);
		current_student = Student::login(students);
		if (current_student.get_id() == -1) {
			system("cls");
			Utils::print("Login failed", ERROR);
			_getch();
			system("cls");
			switch (Utils::create_menu({ "Try again", "Login as Faculty", "Exit" })) {
			case 0:
				system("cls");
				goto login_student_label;
			case 1:
				system("cls");
				goto login_facalty_label;
			case 2:
				return 0;
			} // student login again switch
		}     // student login unsuccessful
		break;
	case 3:
		return 0;
	} // login switch

	// -------------------------------------------------------- //

main_menu_label:
	system("cls");

	if (!is_faculty) {
		Utils::print("Welcome, " + current_student.get_name() + "!\n", BOLD);

		switch (Utils::create_menu(
			{
				"My Profile",
				"View Courses",
				"View Clubs",
				"View Events",
				"View Assignments",
				"Submit an Assignment",
				"Create new Feedback",
				"Enroll in Course",
				"Join Club",
				"Join Event",
			},
			false, true, 25)) {
		case -1:
			Utils::print("You are about to logout\n", WARNING);
			switch (Utils::create_menu({ "Logout", "Cancel" })) {
			case 0:
				goto again;
			} // logout switch
			goto main_menu_label;
		case 0:
		student_profile_label:
			system("cls");
			Student::display_student_heading();
			current_student.display_student();
			Utils::display_column_separator(Student::get_widths());
			switch (Utils::create_menu({ "Change Settings", "Change Password", "Change Phone Number" }, true, true)) {
			case -1:
				goto main_menu_label;
			case 1:
				if (current_student.change_student_password()) {
					system("cls");
					Utils::print("Password changed successfully", SUCCESS);
					students[current_student.get_id()] = current_student;
					Student::refresh(students, courses, assignments, clubs);
					_getch();
				}
				break;
			case 2:
				if (current_student.change_student_phone_number()) {
					system("cls");
					Utils::print("Phone number changed successfully", SUCCESS);
					students[current_student.get_id()] = current_student;
					Student::refresh(students, courses, assignments, clubs);
					_getch();
				}
				break;
			} // profile settings switch
			goto student_profile_label;
		case 1:
			Utils::print("Currently Enrolled Courses\n", BOLD);
			current_student.display_enrolled_courses();
			_getch();
			break;
		case 2:
			Utils::print("Currently Joined Clubs\n", BOLD);
			current_student.display_joined_clubs();
			_getch();
			break;
		case 3:
			Utils::print("Currently Joined Events\n", BOLD);
			current_student.display_club_events();
			_getch();
			break;
		case 4:
			Utils::print("Submitted Assignments");
			current_student.display_submitted_assignments();
			_getch();
			break;
		case 5:
			if (current_student.submit_assignment(assignments)) {
				Utils::print("Assignment submitted successfully", SUCCESS);
				students[current_student.get_id()] = current_student;
				Student::refresh(students, courses, assignments, clubs);
				_getch();
			}
			break;
		case 6:
			if (CourseFeedback::create_course_feedback(current_student, courses, course_feedbacks)) {
				CourseFeedback::refresh(course_feedbacks, students, courses);
				Utils::print("Feedback submitted successfully", SUCCESS);
				_getch();
			}
			_getch();
			break;
		case 7:
			if (current_student.enroll_in_course(courses)) {
				Utils::print("Enrolled successfully", SUCCESS);
				students[current_student.get_id()] = current_student;
				Student::refresh(students, courses, assignments, clubs);
				_getch();
			}
			break;
		case 8:
			if (current_student.join_club(clubs)) {
				Utils::print("Joined club successfully", SUCCESS);
				students[current_student.get_id()] = current_student;
				Student::refresh(students, courses, assignments, clubs);
				Club::refresh(clubs, events);
				_getch();
			}
			break;
		case 9:
			if (current_student.join_event(events)) {
				Utils::print("Joined event successfully", SUCCESS);
				students[current_student.get_id()] = current_student;
				Student::refresh(students, courses, assignments, clubs);
				Event::refresh(events);
				_getch();
			}
		} // student menu switch
		goto main_menu_label;
	} // if not faculty

	// ------------------------------------------------------------ //

	if (is_faculty) {
		// options according to auth_level
		map<float, string> auth_level_options = {
			{1.00, "My Profile"},
			{1.01, "View All Courses"},
			{1.02, "View All Clubs"},
			{1.03, "View All Events"},
			{2.04, "View My Courses"},
			{2.05, "Create Assignment"},
			{2.06, "Edit Assignment"},
			{2.07, "Mark Attendence"},
			{2.08, "Grade Students"},
			{2.09, "Register Student in Course"},
			{2.10, "Unregister Student from Course"},
			{2.11, "Change Faculty Level"},
			{3.12, "Create Course"},
			{3.13, "Edit Course"},
			{3.14, "Create Club"},
			{3.15, "View Feedbacks"},
			{3.16, "Allocate Course to Faculty"},
			{3.17, "Deallocate Course from Faculty"},
			{4.18, "Admit New Student"},
			{4.19, "Admit New Faculty"},
			{4.20, "Register a Department"},
			{4.21, "Display all Tables"},
		};

		Storage1<string> options;
		for (auto& option : auth_level_options)
			if (current_faculty.get_auth_level() >= int(option.first))
				options.push_back(option.second);

		Utils::print("Welcome, " + current_faculty.get_name() + "!\n", BOLD);
		switch (Utils::create_menu(options, false, true, 40)) {

		case -1:
			Utils::print("You are about to logout\n", WARNING);
			switch (Utils::create_menu({ "Logout", "Cancel" })) {
			case 0:
				goto again;
			} // logout switch
			break;
		case 0:
		faculty_profile_label:
			system("cls");
			Faculty::display_headings();
			current_faculty.display_faculty();
			Utils::display_column_separator(Faculty::get_widths());
			switch (Utils::create_menu({ "Change Settings", "Change Password", "Change Phone Number" }, true, true)) {
			case -1:
				goto main_menu_label;
			case 1:
				if (current_faculty.change_faculty_password()) {
					system("cls");
					Utils::print("Password changed successfully", SUCCESS);
					faculties[current_faculty.get_id()] = current_faculty;
					Faculty::refresh(faculties);
					_getch();
				}
				break;
			case 2:
				if (current_faculty.change_faculty_phone_number()) {
					system("cls");
					Utils::print("Phone number changed successfully", SUCCESS);
					faculties[current_faculty.get_id()] = current_faculty;
					Faculty::refresh(faculties);
					_getch();
				}
				break;
			} // profile settings switch
			goto faculty_profile_label;
		case 1:
			Utils::print("Courses\n", BOLD);
			Course::display_all(courses);
			_getch();
			break;
		case 2:
			Utils::print("Clubs\n", BOLD);
			Club::display_all(clubs);
			_getch();
			break;
		case 3:
			Utils::print("Events\n", BOLD);
			Event::display_events(events);
			_getch();
			break;
		case 4:
			FacultyCourseAllocation::display_faculty_courses(current_faculty.get_id(), faculty_course_allocations);
			_getch();
			break;
		case 5:
			if (Assignment::create_assignment(assignments, FacultyCourseAllocation::get_faculty_courses(current_faculty.get_id(), faculty_course_allocations))) {
				Assignment::refresh(assignments, courses);
				Utils::print("Assignment created successfully", SUCCESS);
				_getch();
			}
			break;
		case 6:
			if (Assignment::edit_assignment(assignments, courses)) {
				Assignment::refresh(assignments, courses);
				Utils::print("Assignment edited successfully", SUCCESS);
				_getch();
			}
			break;
		case 7:
			if (Attendence::mark_attendence(attendences, students, FacultyCourseAllocation::get_faculty_courses(current_faculty.get_id(), faculty_course_allocations))) {
				Attendence::refresh(attendences, students, courses);
				Utils::print("Attendence marked successfully", SUCCESS);
				_getch();
			}
			break;
		case 8:
			if (Grading::grade_students(grades, students, FacultyCourseAllocation::get_faculty_courses(current_faculty.get_id(), faculty_course_allocations))) {
				Grading::refresh(grades, students, courses);
				Utils::print("Grades marked successfully", SUCCESS);
				_getch();
			}
			break;
		case 9:
			if (Student::enroll_a_student_to_course(students, courses)) {
				Student::refresh(students, courses, assignments, clubs);
				Utils::print("Student enrolled successfully", SUCCESS);
				_getch();
			}
			break;
		case 10:
			if (Student::unenroll_a_student_from_course(students, courses)) {
				Student::refresh(students, courses, assignments, clubs);
				Utils::print("Student unenrolled successfully", SUCCESS);
				_getch();
			}
			break;
		case 11:
			if (current_faculty.change_faculty_auth_level(faculties)) {
				Faculty::refresh(faculties);
				Utils::print("Changed Faculty Authorization level successfully", SUCCESS);
				_getch();
			}
			break;
		case 12:
			if (Course::create_course(courses, departments)) {
				Course::refresh(courses);
				Utils::print("Course created successfully", SUCCESS);
				_getch();
			}
			break;
		case 13:
			if (Course::update_course(courses, departments)) {
				Course::refresh(courses);
				Utils::print("Course edited successfully", SUCCESS);
				_getch();
			}
			break;
		case 14:
			if (Club::create_club(clubs)) {
				Club::refresh(clubs, events);
				Utils::print("Club created successfully", SUCCESS);
				_getch();
			}
			break;
		case 15:
			Utils::print("Course Feedbacks\n", BOLD);
			CourseFeedback::display_all(course_feedbacks);
			_getch();
			break;
		case 16:
			if (FacultyCourseAllocation::allocate_course_to_faculty(faculty_course_allocations, faculties, courses)) {
				FacultyCourseAllocation::refresh(faculty_course_allocations, faculties, courses);
				Faculty::refresh(faculties);
				Utils::print("Course allocated successfully", SUCCESS);
				_getch();
			}
			break;
		case 17:
			if (FacultyCourseAllocation::deallocate_course_from_faculty(faculty_course_allocations)) {
				FacultyCourseAllocation::refresh(faculty_course_allocations, faculties, courses);
				Utils::print("Course deallocated successfully", SUCCESS);
				_getch();
			}
			break;
		case 18:
			if (Student::admit_a_student(students, departments)) {
				Student::refresh(students, courses, assignments, clubs);
				Utils::print("Student admitted successfully", SUCCESS);
				_getch();
			}
			break;
		case 19:
			if (Faculty::admit_faculty(faculties)) {
				Faculty::refresh(faculties);
				Utils::print("Faculty admitted successfully", SUCCESS);
				_getch();
			}
			break;
		case 20:
			if (Department::register_department(departments, faculties)) {
				Department::refresh(departments);
				Faculty::refresh(faculties);
				Utils::print("Department registered successfully", SUCCESS);
				_getch();
			}
			break;
		case 21:
		all_tables_label:
			system("cls");
			switch (Utils::create_menu({ "Select a table to display", "Faculty", "Department", "Course", "Assignment", "Faculty Course Allocation", "Event", "Club", "Student", "Course Feedback", "Attendence", "Grading" }, true, true, 40)) {
			case -1:
				goto main_menu_label;
			case 1:
				Utils::print("Faculties\n", BOLD);
				Faculty::display_all(faculties);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/faculties.csv");
					_getch();
				}
				break;
			case 2:
				Utils::print("Departments\n", BOLD);
				Department::display_all(departments);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/departments.csv");
					_getch();
				}
				break;
			case 3:
				Utils::print("Courses\n", BOLD);
				Course::display_all(courses);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/courses.csv");
					_getch();
				}
				break;
			case 4:
				Utils::print("Assignments\n", BOLD);
				Assignment::display_all(assignments);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/assignments.csv");
					_getch();
				}
				break;
			case 5:
				Utils::print("Faculty Course Allocation\n", BOLD);
				FacultyCourseAllocation::display_all(faculty_course_allocations);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/faculty_course_allocations.csv");
					_getch();
				}
				break;
			case 6:
				Utils::print("Events\n", BOLD);
				Event::display_events(events);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/events.csv");
					_getch();
				}
				break;
			case 7:
				Utils::print("Clubs\n", BOLD);
				Club::display_all(clubs);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/clubs.csv");
					_getch();
				}
				break;
			case 8:
				Utils::print("Students\n", BOLD);
				Student::display_students(students);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/students.csv");
					_getch();
				}
				break;
			case 9:
				Utils::print("Course Feedbacks\n", BOLD);
				CourseFeedback::display_all(course_feedbacks);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/course_feedbacks.csv");
					_getch();
				}
				break;
			case 10:
				Utils::print("Attendences\n", BOLD);
				Attendence::display_all(attendences);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/attendences.csv");
					_getch();
				}
				break;
			case 11:
				Utils::print("Grades\n", BOLD);
				Grading::display_all(grades);
				cout << endl;
				switch (Utils::create_menu({ "Open in excel", "Back" })) {
				case 0:
					system("start excel data/grades.csv");
					_getch();
				}
				break;
			} // all tables switch
			goto all_tables_label;
		} // faculty menu switch
		goto main_menu_label;
	} // if faculty
	return 0;
} // main