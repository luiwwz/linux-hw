#include "shared_array.h"
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
   shared_array arr("/myarray", 10);
   sem_t* sem = sem_open("/mysem", O_CREAT, 0666, 1);

   while (true) {
      sem_wait(sem);
      arr[0] += 2;
      std::cout << "second: " << arr[0] << '\n';
      sem_post(sem);
      sleep(1);
   }
}

