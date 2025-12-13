#include "shared_array.h"
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    try {
        shared_array arr("/myarray", 10);
        sem_t* sem = sem_open("/mysem", O_CREAT, 0666, 1);
        if (sem == SEM_FAILED) {
            std::cerr << "Failed to open semaphore\n";
            return 1;
        }

        while (true) {
            sem_wait(sem);
            arr[0] += 1;
            std::cout << "first: " << arr[0] << '\n';
            sem_post(sem);
            sleep(1);
        }

        sem_close(sem);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
