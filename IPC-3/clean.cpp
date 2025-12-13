#include <iostream>
#include <sys/mman.h>
#include <semaphore.h>

int main() {
   shm_unlink("/myarray");
   shm_unlink("/mysem");
   std::cout << "cleanip done\n";
}
