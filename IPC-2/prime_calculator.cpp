#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cstdlib>

bool is_prime(int n) {
    if (n < 2) {
        return false;
    }
    for (int i = 2; i*i <= n; i++) {
         if (n % i == 0) {
             return false;
	 }
    }
    return true;
}

int nth_prime(int n) {
    int count = 0;
    int num = 1;
    while (count < n) {
        num++;
        if (is_prime(num)) {
	    count++;
	};
    }
    return num;
}

int main() {
    int pipe_to_child[2];    
    int pipe_to_parent[2]; 

    if (pipe(pipe_to_child) == -1 || pipe(pipe_to_parent) == -1) {
        std::cerr << "Failed to create pipes\n";
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed\n";
        exit(1);
    } else if (pid == 0) {
     
        close(pipe_to_child[1]);   
        close(pipe_to_parent[0]); 

        while (true) {
            int m;
            ssize_t n = read(pipe_to_child[0], &m, sizeof(m));
            if (n <= 0) {
	        break;
	    }

            std::cout << "[Child] Calculating " << m << "-th prime number...\n";
            int prime = nth_prime(m);
            std::cout << "[Child] Sending calculation result of prime(" << m << ")...\n";

            if (write(pipe_to_parent[1], &prime, sizeof(prime)) != sizeof(prime)) {
                std::cerr << "Failed to write from child to parent\n";
                exit(1);
            }
        }

        close(pipe_to_child[0]);
        close(pipe_to_parent[1]);
    } else {

        close(pipe_to_child[0]);   
        close(pipe_to_parent[1]);

        while (true) {
            std::cout << "[Parent] Please enter the number: ";
            std::string input;
            std::getline(std::cin, input);

            if (input == "exit") {
	        break;
	    }

            int m = std::stoi(input);

            std::cout << "[Parent] Sending " << m << " to child process...\n";
            if (write(pipe_to_child[1], &m, sizeof(m)) != sizeof(m)) {
                std::cerr << "Failed to write from parent\n";
                exit(1);
            }

            std::cout << "[Parent] Waiting for response from the child process...\n";
            int result;
            if (read(pipe_to_parent[0], &result, sizeof(result)) != sizeof(result)) {
                std::cerr << "Failed to read from child\n";
                exit(1);
            }

            std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "\n";
        }

        close(pipe_to_child[1]);
        close(pipe_to_parent[0]);
    }

    return 0;
}
