#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "DoubleNine.h"
#include "Encryption.h"
using namespace std;

int main() {
    string s;
    cout << "Input the string: ";
    while (cin >> s) {
        s = Encryption().encrypt(s);
        cout << "After encrypt: " << s;
        s = Encryption().decrypt(s);
        cout << "\nAfter decrypt: " << s;
        cout << "\n\nInput the string: ";
    }
}
