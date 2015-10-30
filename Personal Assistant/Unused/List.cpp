/*
 * List.cpp
 *
 *  Created on: 2015Äê5ÔÂ30ÈÕ
 *      Author: Sine
 */

#include "List.h"
#include <cassert>
#include <iostream>
#include <iomanip>

template<class type, class container, class iterator>
List<type, container, iterator>::List() {
}

template<class type, class container, class iterator>
void List<type, container, iterator>::add(type t, iterator it) {
//    _listlink.push_back( { t, it });
}

template<class type, class container, class iterator>
void List<type, container, iterator>::operator()(
        const container & ctn, bool (*requirement)(iterator & iter),
        void (*print)(iterator & iter)) {
    clear();
    iterator it;
    int mark = 1;
    for (it = ctn.begin(); it != ctn.end(); it++, mark++) {
        std::cout << std::setw(3) << mark;
        (*print)(it);
        add(*it, it);
    }
}

template<class type, class container, class iterator>
type List<type, container, iterator>::operator[](int mark) {
    assert(mark >= 1);
    assert(mark <= _listlink.size());
    return _listlink[mark - 1]._t;
}

template<class type, class container, class iterator>
iterator List<type, container, iterator>::get_iter(int mark) {
    assert(mark >= 1);
    assert(mark <= _listlink.size());
    return _listlink[mark - 1]._it;
}

template<class type, class container, class iterator>
void List<type, container, iterator>::clear() {
    _listlink.clear();
}
