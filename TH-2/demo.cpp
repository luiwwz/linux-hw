#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

void simple_task(void* arg) {
    int task_id = *(int*)arg;
    std::cout << "Task " << task_id << " running\n";
    sleep(1);
    delete (int*)arg;
}

int main() {
    std::cout << "Creating thread pool with 3 threads\n";
    
    parallel_scheduler scheduler(3);
    
    std::cout << "Adding 10 tasks\n";
    
    for (int i = 1; i <= 10; i++) {
        int* task_id = new int(i);
        scheduler.run(simple_task, task_id);
    }
   
    sleep(5);
    
    std::cout << "Done\n";
    
    return 0;
}
