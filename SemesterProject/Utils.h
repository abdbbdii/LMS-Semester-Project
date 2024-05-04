#pragma once

#include <conio.h>
#include <iomanip>
#include <iostream>
#include <string>
#include "Storage.h"
#include <map>

using namespace std;

// ------------------------------------------------------------------------------------------------------------ //
// ANSI Escape Sequences for colors, cursor movement and text formatting
// Source https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
#define MOVEUP "\033[A"                         // Move cursor up
#define CLEARLINE "\033[2K"                     // Clear the entire line
#define BOLD "\033[1m"                          // Bold text
#define UNDERLINE "\033[4m"                     // Underline text
#define INPUTFIELD "\033[38;5;0m\033[48;5;230m" // Background color (light gray)
#define SELECTED "\033[38;5;0m\033[48;5;111m"   // Background color (cyan)
#define UNSELECTED "\033[48;5;235m"             // Background color (dark gray)
#define BORDER "\033[1m\033[48;5;237m"          // Background color (gray)
#define ERROR "\033[91m"                        // Error color (red)
#define SUCCESS "\033[92m"                      // Success color (green)
#define WARNING "\033[93m"                      // Warning color (yellow)
#define INFO "\033[94m"                         // Info color (blue)
#define RESET "\033[0m"                         // Reset all attributes
// ------------------------------------------------------------------------------------------------------------ //

class Utils {
public:
    static void print(const string& str, const string& format = RESET, const int& set_width = 0, const bool& centered = false, bool endline = true) {
        string pre_text = "";
        if (format == ERROR)
            pre_text = "[ERROR] ";
        else if (format == SUCCESS)
            pre_text = "[SUCCESS] ";
        else if (format == WARNING)
            pre_text = "[WARNING] ";
        else if (format == INFO)
            pre_text = "[INFO] ";
        if (centered && set_width)
            cout << format << left << setw(set_width) << string((set_width - str.length() - pre_text.length()) / 2, ' ') + pre_text + str << RESET << ((endline) ? "\n" : "");
        else if (set_width)
            cout << format << left << setw(set_width) << pre_text + str << RESET << ((endline) ? "\n" : "");
        else
            cout << format << pre_text + str << RESET << ((endline) ? "\n" : "");
    }

    static void remove_last_lines(const int& n) {
        if (n == 0)
            return;
        for (int i = 0; i < n; i++) {
            cout << CLEARLINE << MOVEUP;
        }
        cout << CLEARLINE;
    }

    static int create_menu(const Storage1<string>& arr, const bool& heading = false, const bool& back = false, const int& set_width = 20) {
        size_t menu_size = arr.size() - ((heading) ? 1 : 0); // when using string ptr, change 1 and 0 to 2 and 1 respectively
        int selected = 0;
        int count_extra_lines = 0;
        char key;
        if (back) {
            print("< ESC");
            cout << endl;
            count_extra_lines += 2;
        }
        if (heading) {
            print(arr[0], BOLD, set_width, true);
            cout << endl;
            count_extra_lines += 2;
        }
        do {
            for (int i = 0; i < menu_size; i++) {
                string str = (heading) ? arr[i + 1] : arr[i];
                if (selected == i)
                    print(' ' + str, SELECTED, set_width);
                else
                    print(' ' + str, UNSELECTED, set_width);
            }
            key = _getch();
            remove_last_lines(menu_size);
            if (selected > 0 && key == 72) // up_arrow_key
                selected--;
            else if (selected < menu_size - 1 && key == 80) // down_arrow_key
                selected++;
            else if (selected == 0 && key == 72) // up_arrow_key
                selected = menu_size - 1;
            else if (selected == menu_size - 1 && key == 80) // down_arrow_key
                selected = 0;
            else if (key == 13) { // enter_key
                remove_last_lines(count_extra_lines);
                return (heading) ? selected + 1 : selected;
            }
        } while ((back) ? key != 27 && key != 13 : true);
        remove_last_lines(count_extra_lines);
        return -1;
    }

    template <typename T>
    static T option_selector(const map<T, string>& options, const string& message, const int& set_width = 20) {
        Storage1<string> optionList;
        optionList.push_back(message);
        for (const auto& option : options)
            optionList.push_back(option.second);
        int selected = create_menu(optionList, true, false, set_width);
        return next(options.begin(), selected - 1)->first;
    }

    static string get_input(const string& message, const int& set_width = 20, const bool& centered = false, const bool& password = false) {
        string input = "";
        char key;
        print(((centered) ? "" : " ") + message, INPUTFIELD, set_width, centered, centered);
        if (!centered)
            cout << "\t";
        do {
            key = _getch();
            if (key == 13) // enter_key
                break;
            else if (key == 8) { // backspace_key
                if (input.length() > 0) {
                    input.pop_back();
                    cout << "\b \b";
                }
            }
            else if (key == 27) { // escape_key
                input = "";
                break;
            }
            else {
                input += key;
                cout << (password ? '*' : key);
            }
        } while (true);
        cout << endl;
        return input;
    }

    static Storage1<string> split(const string& str, const char& delimiter) {
        Storage1<string> tokens;
        string token = "";
        for (char c : str) {
            if (c == delimiter) {
                tokens.push_back(token);
                token = "";
            }
            else
                token += c;
        }
        tokens.push_back(token);
        return tokens;
    }

    static void display_column_separator(const Storage1<int>& column_widths) {
        print(" ", BORDER, 0, false, false);
        for (int i = 0; i < column_widths.size(); i++) {
            print(" ", BORDER, 0, false, false); // +
            for (int j = 0; j < column_widths[i]; j++) {
                print(" ", BORDER, 0, false, false); // -
            }
        }
        print("  ", BORDER); // +\n
    }

    static void display_column(const Storage1<string>& column, const Storage1<int>& column_widths, const bool& heading = false, const bool& centered = true) {
        int column_size = 0;
        for (int i = 0; i < column.size(); i++)
            column_size += column_widths[i];

        print("  ", BORDER, 0, false, false); // |
        for (int i = 0; i < column.size(); i++) {
            print(column[i], (heading) ? BORDER : UNSELECTED, column_widths[i], centered, false);
            print(" ", BORDER, 0, false, false); // |
        }
        print(" ", BORDER, 0, false);
    }
};