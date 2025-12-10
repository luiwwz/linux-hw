#pragma once

#include <cstddef>

class shared_array {
private:
   int* data; 
   size_t* size;
public:
   shared_array (const char* name, size_t size_);
   int& operator[](size_t i);
};
