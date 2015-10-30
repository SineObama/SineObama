/*
 * List.h
 *
 *  Created on: 2015Äê5ÔÂ30ÈÕ
 *      Author: Sine
 */

// used to list the list, abandon now

#ifndef LIST_H_
#define LIST_H_

#include <vector>
#include "DynamicArray.h"

template<class type, class container, class iterator>
class List {

 public:

    List();
    void add(type ptr, iterator iter);
    void operator()(const container & ctn,
                    bool (*requirement)(iterator & iter),
                    void (*print)(iterator & iter));
    type operator[](int mark);
    iterator get_iter(int mark);
    void clear();

 private:

    struct ListLink {
        type _t;
        iterator _it;
    };
    std::vector<ListLink> _listlink;

};

#endif /* LIST_H_ */
