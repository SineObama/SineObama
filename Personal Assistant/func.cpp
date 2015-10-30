/*
 * func.cpp
 *
 *  Created on: 2015Äê6ÔÂ1ÈÕ
 *      Author: Sine
 */

#include "func.h"
#include <iostream>
#include <sstream>
#include <conio.h>

void print_order() {
    message("Please enter the number of orders:");
    message("1. Add...");
    message("2. List...");
    message("3. Sort...");
    message("4. Mark an affair as done");
    message("   (need list first and affairs exist)");
    message("5. Mark an affair as todo (just like above)");
    message("6. Delete an affair (just like above)");
    message("7. Delete all done affair");
    message("8. Delete all affair");
    message("0. Exit (also means going back)");
    std::cout << std::endl;
}

// the second number of order 4 and 5 hasn't check yet
order get_order() {
    std::string order = getline_string();
    if (order == "1")
        return _add;
    else if (order == "2")
        return _list;
    else if (order == "3")
        return _sort;
    else if (order == "4")
        return _mark;
    else if (order == "5")
        return _mark_back;
    else if (order == "6")
        return _delete;
    else if (order == "7")
        return _delete_done;
    else if (order == "8")
        return _delete_all;
    else if (order == "0")
        return _exit_;
    else
        return _wrong_order;
}

// get the data for the class Affair
Affair NewAffair() {
    message("Enter the date (like 2015 1 1):");
    Date date(getline_string());

    message("Enter the theme:");
    std::string theme = getline_string();

    message("Enter the detail (ignorable):");
    std::string detail = getline_string();

    return Affair(date, theme, detail);
}

// get a line, store in a string.
// the left charactors in the input stream are abandoned.
std::string getline_string() {
    std::string str;
    std::cin.sync();
    std::getline(std::cin, str);
    return str;
}

void warning_message(const std::string & m) {
    std::cout << WARNING_CHARACTOR << WARNING_CHARACTOR << m << std::endl;
}

// used to help seeing the warning message before going back to the base menu
void my_pause() {
    message("Press any key to continue...");
    getch();
    std::cout << std::endl << std::endl;
}

void message(const std::string & s) {
    std::cout << s << std::endl;
}

void get_number_and_(PA & pa, void (*p)(PA &, int)) {
    if (pa.get_listlink().empty()) {
        warning_message("The list is invalid. "
                        "List first (if you have)");
        my_pause();
        return;
    }
    message("Enter the number of affair in the last list above:");
    std::string _str;
    int num;
    while (DO_UNTIL_BREAK) {
        _str = getline_string();
        if (_str == "0") {
            return;
        } else {
            std::stringstream _stream;
            _stream << _str;
            _stream >> num;
            if (_stream.fail()) {
                warning_message("Read the number fail "
                                "(maybe too long). Again:");
                continue;
            } else {
                (*p)(pa, num);
                return;
            }
        }
    }
}

void do_mark(PA & pa, int num) {
    pa.MarkAsDone(num);
}

void do_mark_back(PA & pa, int num) {
    pa.MarkAsTodo(num);
}

void do_delete(PA & pa, int num) {
    pa.Delete(num);
}
