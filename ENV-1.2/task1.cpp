#include <iostream>
#include <cstdlib> // для atoi

using namespace std;

int main(int argc, char* argv[]) {
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = atoi(argv[3]);

    int max = a, min = a;

    if (b > max) max = b;
    if (c > max) max = c;

    if (b < min) min = b;
    if (c < min) min = c;

    cout << "min-" << min << ", max-" << max << endl;

    return 0;
}
