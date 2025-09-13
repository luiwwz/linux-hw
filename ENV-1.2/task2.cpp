#include <iostream>

int main(int argc, char* argv[]) {
    for (int i = argc - 1; i >= 1; --i) {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
