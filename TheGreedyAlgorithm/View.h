/*
 * View.h
 *
 *  Created on: 2016年10月18日
 *      Author: Sine
 */

#ifndef VIEW_H_
#define VIEW_H_

#include <set>

using namespace std;

class View {
 public:
    View() {
        choosed = counted = false;
        curCost = cost = id = 0;  // 没有意义
    }
    int getB(int newCost) {
        if (counted || newCost >= curCost)
            return 0;
        counted = true;
        int sum = curCost - newCost;
        for (set<View *>::iterator iter = childs.begin(); iter != childs.end();
                iter++) {
            sum += (*iter)->getB(newCost);
        }
        return sum;
    }
    void refreshCost(int newCost) {
        if (newCost >= curCost)
            return;
        curCost = newCost;
        for (set<View *>::iterator iter = childs.begin(); iter != childs.end();
                iter++) {
            (*iter)->refreshCost(newCost);
        }
    }
    void clearCounted() {
        if (!counted)
            return;
        counted = false;
        for (set<View *>::iterator iter = childs.begin(); iter != childs.end();
                iter++) {
            (*iter)->clearCounted();
        }
    }
    bool choosed;
    int id;
    int cost;
    int curCost;
    bool counted;
    set<View *> childs;
};

#endif /* VIEW_H_ */
