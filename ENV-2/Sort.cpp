#include "Sort.h"

void bubbleSort(std::vector<Complex>& arr) {
	for(size_t i = 0; i < arr.size(); i++) {
		for(size_t j = 0; j < arr.size() - i - 1; j++) {
			if(arr[j].abs() > arr[j+1].abs()) {
				std::swap(arr[j], arr[j+1]);
			}
		}
	}
}
