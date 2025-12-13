#include "shared_array.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

shared_array::shared_array(shared_array&& other) noexcept
    : data(other.data), size_(other.size_), shm_name(other.shm_name) {
    other.data = nullptr;
    other.size_ = 0;
    other.shm_name = nullptr;
}

shared_array::shared_array(const char* name, size_t size_)
    : shm_name(name), size_(size_) {
    if (size_ == 0 || size_ > 1'000'000'000) {
        throw std::invalid_argument("Size out of range");
    }

    int fd = shm_open(name, O_RDWR | O_CREAT, 0666);
    if (fd < 0) { 
	    throw std::runtime_error("Failed to open shared memory");
    }

    struct stat st;
    if (fstat(fd, &st) == 0 && st.st_size != 0 && (size_t)st.st_size != size_ * sizeof(int)) {
        close(fd);
        shm_unlink(name);
        throw std::runtime_error("Existing shared memory has different size");
    }

    if (ftruncate(fd, size_ * sizeof(int)) != 0) {
        close(fd);
        shm_unlink(name);
        throw std::runtime_error("Failed to set size of shared memory");
    }

    data = (int*)mmap(nullptr, size_ * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        shm_unlink(name);
        throw std::runtime_error("Failed to mmap shared memory");
    }

    close(fd);
}

shared_array::~shared_array() {
    if (data) {
        munmap(data, size_ * sizeof(int));
    }
}

int& shared_array::operator[](size_t i) {
    if (!data || i >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[i];
}

void shared_array::remove() {
    if (shm_name) {
        shm_unlink(shm_name);
    }
}

size_t shared_array::size() const {
    return size_;
}
