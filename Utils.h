// Utils.h 
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

inline int randInt(int n) {
    return rand() % n;
}

inline int readInt(const string& prompt, int lo, int hi) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) {
            cin.ignore(10000, '\n');
            if (v >= lo && v <= hi) return v;
        } else {
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cout << "  Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

inline bool readYesNo(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (cin >> s) {
            cin.ignore(10000, '\n');
            if (s[0] == 'y' || s[0] == 'Y') return true;
            if (s[0] == 'n' || s[0] == 'N') return false;
        } else {
            cin.clear();
        }
        cout << "  Please answer y or n.\n";
    }
}

// ===== Function templates =====
template <typename T>
void swapValues(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename T>
void shuffleArray(vector<T>& arr) {
    for (size_t i = arr.size(); i > 1; --i) {
        size_t j = rand() % i;
        swap(arr[i - 1], arr[j]);
    }
}

#endif
