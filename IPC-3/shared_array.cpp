#include "shared_array.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


shared_array::shared_array(const char* name, size_t size_) {
   int fd = shm_open(name, O_RDWR | O_CREAT, 0666);
   ftruncate(fd, size_ * sizeof(int));
   data = (int*) mmap(nullptr, size_ * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   close(fd);
}

int& shared_array::operator[](size_t i) {
   return data[i];
}
