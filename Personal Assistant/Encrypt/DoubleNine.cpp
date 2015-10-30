/*
 * DoubleNine.cpp
 *
 *  Created on: 2015Äê6ÔÂ12ÈÕ
 *      Author: Sine
 */

// you ought not to see this file

#include "DoubleNine.h"
#include "D:\\others\\workspace\\sine_debug.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <stdexcept>

DoubleNine::DoubleNine(const int order[]) {
    for (int i = 0; i < 5; i++)
        _order[i] = order[i];
    checkorder();
    _nu = _nd = _ml = _mr = 0;
}

DoubleNine::DoubleNine(int a, int b, int c, int d, int e) {
    _order[0] = a;
    _order[1] = b;
    _order[2] = c;
    _order[3] = d;
    _order[4] = e;
    checkorder();
    _nu = _nd = _ml = _mr = 0;
}

void DoubleNine::setpoint(int n1, int m1, int n2, int m2) {
    if (!(n1 > 0 && n1 < ROW && n2 > 0 && n2 < ROW && m1 > 0 && m1 < ROW
            && m2 > 0 && m2 < ROW))
        throw std::invalid_argument("DoubleNine::setpoint(): out of 1~ROW-1");

    if (n1 < n2) {
        _nu = n1;
        _nd = n2;
    } else {
        _nu = n2;
        _nd = n1;
    }
    if (m1 < m2) {
        _ml = m1;
        _mr = m2;
    } else {
        _ml = m2;
        _mr = m1;
    }
}

unsigned int DoubleNine::getsize() {
    return _nu * _ml + _nu * (ROW - _mr) + (_nd - _nu) * (_mr - _ml)
            + (ROW - _nd) * _ml + (ROW - _nd) * (ROW - _mr);
}

std::string DoubleNine::encrypt(const std::string & in) {
    if (in.size() > getsize())
        throw std::invalid_argument("DoubleNine::encrypt(): string too long.");

    char map[ROW][ROW] = { };
    writetomap(in, map);

    std::string out;
    out.reserve(in.size() + 1);
    int pcount = 0;
    while (pcount < 5) {
        int nu = 0, nd = ROW, ml = 0, mr = ROW;
        switch (_order[pcount++]) {
            case 1:
                nd = _nu;
                mr = _ml;
                break;
            case 2:
                nd = _nu;
                ml = _mr;
                break;
            case 3:
                nu = _nu;
                nd = _nd;
                ml = _ml;
                mr = _mr;
                break;
            case 4:
                nu = _nd;
                mr = _ml;
                break;
            case 5:
                nu = _nd;
                ml = _mr;
                break;
            default:
                throw std::out_of_range(
                        "Double Nine: order invalid. (this should not "
                        "happen, please contact the author.)");
        }  // end switch. part's bounds are set

        for (int j = ml; j < mr; j++) {
            for (int i = nu; i < nd && map[i][j] != 0; i++) {
                out += map[i][j];
            }
        }
    }
    return out;
}

std::string DoubleNine::decrypt(const std::string & in) {
    if (in.size() > getsize())
        throw std::invalid_argument("DoubleNine::decrypt(): string too long.");

    char map[ROW][ROW] = { };
    writetomap(in, map);

    int pcount = 0, rcount = 0;
    while (pcount < 5) {
        int nu = 0, nd = ROW, ml = 0, mr = ROW;
        switch (_order[pcount++]) {
            case 1:
                nd = _nu;
                mr = _ml;
                break;
            case 2:
                nd = _nu;
                ml = _mr;
                break;
            case 3:
                nu = _nu;
                nd = _nd;
                ml = _ml;
                mr = _mr;
                break;
            case 4:
                nu = _nd;
                mr = _ml;
                break;
            case 5:
                nu = _nd;
                ml = _mr;
                break;
            default:
                throw std::out_of_range(
                        "Double Nine: order invalid. (this should not "
                        "happen, please contact the author.)");
        }  // end switch. part's bounds are set

        for (int j = ml; j < mr; j++) {
            for (int i = nu; i < nd && map[i][j] != 0; i++) {
                map[i][j] = in[rcount++];
            }
        }
    }
    return readfrommap(map);
}

bool DoubleNine::checkorder() {
    bool check[5] = { };
    for (int i = 0; i < 5; i++) {
        if (_order[i] < 1 || _order[i] > 5 || check[_order[i] - 1])
            throw std::invalid_argument(
                    "DoubleNine: order invalid. (constructor)");
        else
            check[_order[i] - 1] = true;
    }
    return true;
}

void DoubleNine::writetomap(const std::string & in, char map[][ROW]) {
    int len = in.size();
    int ccount = 0;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < ROW; j++) {
            if (i < _nu || i >= _nd) {
                if (j >= _ml && j < _mr)
                    continue;
            } else if (j < _ml || j >= _mr) {
                continue;
            }
            map[i][j] = in[ccount++];
            if (ccount >= len)
                return;
        }
    }
}

std::string DoubleNine::readfrommap(const char map[][ROW]) {
    std::string out;
    out.reserve(getsize() + 1);
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < ROW; j++) {
            if (i < _nu || i >= _nd) {
                if (j >= _ml && j < _mr)
                    continue;
            } else if (j < _ml || j >= _mr) {
                continue;
            }
            if (map[i][j] != 0)
                out += map[i][j];
        }
    }
    return out;
}
