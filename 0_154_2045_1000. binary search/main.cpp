#include <vector>
#include <iostream>
using namespace std;

template<typename T>
int bs(const vector<T> t, int bot, int top, const T &x) {
    if (top == bot) {
        if (t[top] == x)
            return top;
        return -1;
    }
    int m = (bot + top) / 2;
    if (t[m + 1] > x)
        return bs(t, bot, m, x);
    return bs(t, m + 1, top, x);
}

template<typename T>
int binarySearch(const vector<T> t, int bot, int top, const T &x) {
    if (x > t[top] || x < t[bot])
        return -1;
    return bs(t, bot, top, x);
}

int main() {
    int a[] = { 0, 1, 1, 3, 3, 3, 6 };

    vector<int> v(a, a + 7);

    cout << binarySearch(v, 0, v.size() - 1, 3);  // Êä³ö5

    cout << binarySearch(v, 0, v.size() - 1, 7);  //Êä³ö-1

}
