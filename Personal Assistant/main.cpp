/*
 * main.cpp
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#include "PA.h"
#include <iostream>
#include <sstream>
#include "func.h"
using namespace std;

//#define NDEBUG

// fixme multiply delete
// fixme condition that can not exit?
// fixme everywhere need a way to go back
int main() {
    message("Welcome to Personal Assistant.");
    message("------------------------------");  // 30 -s
    message("Please enter your name:");
    Affair().mark_done();
    std::string name;
    getline(std::cin, name);
    PA pa(name);
    message("Hello, " + name);

    while (DO_UNTIL_BREAK) {
        message("------------------------------");
        print_order();
        order _order = get_order();

        std::string _str;  // for further orders
        switch (_order) {
            // number 1
            case _add:
                message("Enter the type of affair (\"e\" for event");
                message("                      and \"t\" for task):");
                while (DO_UNTIL_BREAK) {
                    _str = getline_string();
                    if (_str == "0") {
                        break;
                    } else if (_str == "e") {
                        pa.AddEvent(NewAffair());
                        break;
                    } else if (_str == "t") {
                        pa.AddTask(NewAffair());
                        break;
                    } else {
                        warning_message("The type name is wrong. Again:");
                        continue;
                    }
                }
                break;

                // number 2
            case _list:
                message("Enter the number of list:");
                message("1. List all.");
                message("2. List all events.");
                message("3. List all tasks.");
                message("4. List all todos.");
                message("5. List all dones.");
                while (DO_UNTIL_BREAK) {
                    _str = getline_string();
                    if (_str == "0") {
                        break;
                    } else if (_str == "1") {
                        pa.ListAll();
                        my_pause();
                        break;
                    } else if (_str == "2") {
                        pa.ListEvent();
                        my_pause();
                        break;
                    } else if (_str == "3") {
                        pa.ListTask();
                        my_pause();
                        break;
                    } else if (_str == "4") {
                        pa.ListTodos();
                        my_pause();
                        break;
                    } else if (_str == "5") {
                        pa.ListDone();
                        my_pause();
                        break;
                    } else {
                        warning_message("The input is wrong. Again:");
                        continue;
                    }
                }
                break;

                // number 3
            case _sort:
                message("Enter the number of sort according to the time:");
                message("1. Ascending.");
                message("2. Descending.");
                while (DO_UNTIL_BREAK) {
                    _str = getline_string();
                    if (_str == "0") {
                        break;
                    } else if (_str == "1") {
                        pa.SortAscending();
                        message("Sort ascending successfully.");
                        my_pause();
                        break;
                    } else if (_str == "2") {
                        pa.SortDescending();
                        message("Sort descending successfully.");
                        my_pause();
                        break;
                    } else {
                        warning_message("The input is wrong. Enter again:");
                    }
                }
                break;

                // number 4
            case _mark:
                get_number_and_(pa, do_mark);
                break;

                // number 5
            case _mark_back:
                get_number_and_(pa, do_mark_back);
                break;

                // number 6
            case _delete:
                get_number_and_(pa, do_delete);
                break;

                // number 7
            case _delete_done:
                warning_message("Sure to delete all done affairs? "
                                "(\"Y\" to confirm)");
                if (getline_string() == "Y") {
                    pa.DeleteAllDone();
                    warning_message("All done affairs are deleted.");
                    my_pause();
                }
                break;

                // number 8
            case _delete_all:
                warning_message("Sure to delete all affairs? "
                                "(\"Y\" to confirm)");
                if (getline_string() == "Y") {
                    pa.DeleteAll();
                    message("All affairs are deleted.");
                    my_pause();
                }
                break;

                // number 0
            case _wrong_order:
                warning_message("The order is wrong...");
                my_pause();
                break;

            case _exit_:
                break;
        }  // end order select

        if (_order == _exit_)
            break;
        std::cout << std::endl;
    }  // end main loop

    message("Programme exit. Goodbye " + name + ".");
}  // end main
