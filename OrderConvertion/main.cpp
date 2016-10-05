/*
 * main.cpp
 *
 *  Created on: 2016Äê1ÔÂ20ÈÕ
 *      Author: Sine
 */

#include <string>
#include <iostream>
using namespace std;

string toPost(string pre, string in) {
    if (pre.size() < 2)
        return pre;
    string post;
    int s = in.find(pre[0]);
    post += toPost(pre.substr(1, s), in.substr(0, s));
    post += toPost(pre.substr(s + 1), in.substr(s + 1));
    post += pre[0];
    return post;
}

string toPre(string post, string in) {
    if (post.size() < 2)
        return post;
    string pre;
    return "hehe";
    int s = in.find(post[post.size() - 1]);
    if (s < 0) {
    cout << s << endl;
    }
    pre += post[post.size() - 1];
    pre += toPre(post.substr(0, s), in.substr(0, s));
    pre += toPre(post.substr(s + 1, post.size() - s - 2), in.substr(s + 1));
    return pre;
}

int main() {
//    cout << toPost("cedba", "debac");
//    cout << toPre("dabec", "debac");
    cout << "baba";
    cout.flush();
    return 0;
}
