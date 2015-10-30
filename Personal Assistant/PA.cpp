/*
 * Personal Assistant.cpp
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#include "PA.h"
#include "msg.h"
#include "func.h"
#include "Affair.h"
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

PA::PA(const std::string & name)
        : _name(name) {
}

void PA::AddEvent(const Affair & affair) {
    _affair.push_back(new Event(affair));
}

void PA::AddTask(const Affair & affair) {
    _affair.push_back(new Task(affair));
}

const std::map<int, PA::value_iterator> &PA::get_listlink() const {
    return _listlink;
}

void PA::ListAll() {
    list_basic(is_Any);
}

void PA::ListEvent() {
    list_basic(is_Event);
}

void PA::ListTask() {
    list_basic(is_Task);
}

void PA::ListTodos() {
    list_basic(is_Todo);
}

void PA::ListDone() {
    list_basic(is_Done);
}

void PA::SortAscending() {
    sort(_affair.begin(), _affair.end(), before);
    _listlink.clear();
}

void PA::SortDescending() {
    sort(_affair.begin(), _affair.end(), later);
    _listlink.clear();
}

void PA::MarkAsDone(unsigned mark) {
    if (_listlink.empty()) {
        warning_message("The list has not listed or it's invalid.");
        my_pause();
        return;
    }

    map_iterator it = _listlink.find(mark);
    if (it == _listlink.end()) {
        warning_message("The number doesn't exist in the list.");
        my_pause();
        return;
    }
    (*(it->second))->mark_done();
}

void PA::MarkAsTodo(unsigned mark) {
    if (_listlink.empty()) {
        warning_message("The list has not listed or it's invalid.");
        my_pause();
        return;
    }

    map_iterator it = _listlink.find(mark);
    if (it == _listlink.end()) {
        warning_message("The number doesn't exist in the list.");
        my_pause();
        return;
    }
    (*(it->second))->mark_todo();
}

void PA::Delete(unsigned mark) {
    if (_listlink.empty()) {
        warning_message("The list has not listed or it's invalid.");
        my_pause();
        return;
    }

    map_iterator it = _listlink.find(mark);
    if (it == _listlink.end()) {
        warning_message("The number doesn't exist in the list.");
        my_pause();
        return;
    }
    delete *(it->second);
    _affair.erase(it->second);
    _listlink.erase(it);
}

void PA::DeleteAllDone() {
    value_iterator it;
    for (it = _affair.begin(); it != _affair.end(); it++)
        if (is_Done(it)) {
            delete *it;
            *it = nullptr;
            it = _affair.erase(it);
        }
    _listlink.clear();
}

void PA::DeleteAll() {
    value_iterator it;
    for (it = _affair.begin(); it != _affair.end(); it++) {
        delete *it;
        *it = nullptr;
    }
    _listlink.clear();
}

PA::~PA() {
    DeleteAll();
}

void PA::list_basic(bool (*judge)(value_iterator)) {
    _listlink.clear();
    std::cout << std::endl
              << "|Mark Done Type     Time     \004 Theme  \004 detail"
              //  |   1  ¡Ì¡Ì  Event 2015-06-01  \004 .....  \004 ......
              //  | 4 ||  6 ||  6 ||   10   ||  4  ||...||  4  ||....|
            << std::endl;
    if (!_affair.empty()) {
        int mark = 1;
        value_iterator it;
        for (it = _affair.begin(); it != _affair.end(); it++) {
            if ((*judge)(it)) {
                std::cout << "|" << std::setw(4) << mark
                          << (is_Done(it) ? "  ¡Ì  " : "      ");
                (*it)->print();
                std::cout << std::endl;
                _listlink.insert(make_pair(mark++, it));
            }
        }
    }
    std::cout << std::endl;
}

bool PA::is_Any(value_iterator it) {
    return true;
}

bool PA::is_Event(value_iterator it) {
    return (*it)->type() == "Event";
}

bool PA::is_Task(value_iterator it) {
    return (*it)->type() == "Task";
}

bool PA::is_Todo(value_iterator it) {
    return !(*it)->done();
}

bool PA::is_Done(value_iterator it) {
    return (*it)->done();
}

bool PA::later(Affair *a, Affair *b) {
    return a->get_date() > b->get_date();
}

bool PA::before(Affair *a, Affair *b) {
    return a->get_date() < b->get_date();
}
