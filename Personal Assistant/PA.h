/*
 * Personal Assistant.h
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#ifndef PA_H_
#define PA_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "Event.h"
#include "Task.h"
#include "Affair.h"

// mark todos

#define nullptr NULL

class PA {

 public:

    typedef std::vector<Affair*> value_type;
    typedef value_type::iterator value_iterator;
    typedef std::map<int, value_iterator> map_type;
    typedef map_type::iterator map_iterator;

    PA(const std::string & name);

    void AddEvent(const Affair & affair);
    void AddTask(const Affair & affair);

    // for checking the list.
    const map_type &get_listlink() const;
    void ListAll();
    void ListEvent();
    void ListTask();
    void ListTodos();
    void ListDone();

    /* after sort or delete all (done), the list is invalid for mark as done and delete
     * for the number has changed */

    // sort according to the date, the "list" will be cleared
    void SortAscending();
    void SortDescending();

    // the number is which was shown in the list before calling this three
    void MarkAsDone(unsigned mark);
    void MarkAsTodo(unsigned mark);
    void Delete(unsigned mark);

    // the "list" will be cleared
    void DeleteAllDone();
    void DeleteAll();

    ~PA();

 protected:

    void list_basic(bool (*judge)(value_iterator));
    static bool is_Any(value_iterator it);
    static bool is_Event(value_iterator it);
    static bool is_Task(value_iterator it);
    static bool is_Todo(value_iterator it);
    static bool is_Done(value_iterator it);

    // comparison of date, used in sorting
    static bool later(Affair *a, Affair *b);
    static bool before(Affair *a, Affair *b);

 private:

    std::string _name;
    value_type _affair;

    // serve as the list, with a number corresponding to an value_iterator
    map_type _listlink;

};

#endif /* PA_H_ */
