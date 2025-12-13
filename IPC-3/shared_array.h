#pragma once

#include <cstddef>

class shared_array {
private:
    int* data;
    size_t size_;
    const char* shm_name;

public:
    shared_array(const shared_array&) = delete;
    shared_array& operator=(const shared_array&) = delete;

    shared_array(shared_array&& other) noexcept;

    shared_array(const char* name, size_t size_);

    ~shared_array();

    int& operator[](size_t i);

    void remove();

    size_t size() const;
};
