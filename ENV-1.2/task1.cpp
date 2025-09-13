#include <iostream>
#include <stdexcept>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " num1 num2 num3" << endl;
        return 1;
    }

    int a, b, c;

    try {
        a = stoi(argv[1]);
        b = stoi(argv[2]);
        c = stoi(argv[3]);
    } catch (const invalid_argument& e) {
        cout << "Error: One of the inputs is not a valid integer." << endl;
        return 1;
    } catch (const out_of_range& e) {
        cout << "Error: One of the inputs is out of the valid integer range." << endl;
        return 1;
    }

    int max = a, min = a;

    if (b > max) max = b;
    if (c > max) max = c;

    if (b < min) min = b;
    if (c < min) min = c;

    cout << "min-" << min << ", max-" << max << endl;

    return 0;
}
