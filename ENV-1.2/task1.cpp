#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

bool isNumber(const string& s, int& num) {
    if (s.empty()) return false;

    for (char c : s) {
        if (!isdigit(c)) return false;
    }

    try {
        num = stoi(s);
    } catch (const out_of_range&) {
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " num1, num2, num3"  << endl;
        return 1;
    }

    int a, b, c;

    if (!isNumber(argv[1], a) || !isNumber(argv[2], b) || !isNumber(argv[3], c)) {
        cout << "Error: all arguments must be valid integers within range!" << endl;
        return 1;
    }

    int min = a;
    int max = a;

    if (b > max) max = b;
    if (c > max) max = c;

    if (b < min) min = b;
    if (c < min) min = c;

    cout << "min-" << min << ", max-" << max << endl;
}
