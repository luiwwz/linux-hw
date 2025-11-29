#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>
#include <queue>

struct Task {
   void (*function)(void*);
   void* argument;   
};

class parallel_scheduler {
private:
   pthread_t* threads;
   int cap;
   std::queue<Task> tasks;
   pthread_mutex_t queue_mutex; 
   pthread_cond_t queue_cond;
   bool shutdown;

   static void* work_thread(void* arg);

   void add_task(const Task& task);

   bool remove_task(Task& task);

public:
   parallel_scheduler(int thread_cap);
 
   void run(void (*function)(void*), void* argument);

   ~parallel_scheduler();  
};

#endif
