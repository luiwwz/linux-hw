#include "parallel_scheduler.h"
#include <iostream>

parallel_scheduler::parallel_scheduler(int thread_cap): cap(thread_cap) {
   shutdown = false;

   pthread_mutex_init(&queue_mutex, nullptr);
   pthread_cond_init(&queue_cond, nullptr);

   threads = new pthread_t[cap];

   for (int i = 0; i < cap; i++) {
        pthread_create(&threads[i], nullptr, work_thread, this);
   }
}

parallel_scheduler::~parallel_scheduler() {
   pthread_mutex_lock(&queue_mutex);
   shutdown = true;
   pthread_mutex_unlock(&queue_mutex);

   pthread_cond_broadcast(&queue_cond);

   for (int i = 0; i < cap; i++) {
        pthread_join(threads[i], nullptr);
   }

   delete[] threads;
   pthread_mutex_destroy(&queue_mutex);
   pthread_cond_destroy(&queue_cond);
}

void parallel_scheduler::run(void (*function)(void*), void*argument) {
   Task task;
   task.function = function;
   task.argument = argument;

   add_task(task);
}

void parallel_scheduler::add_task(const Task& task) {
   pthread_mutex_lock(&queue_mutex);

   tasks.push(task);

   pthread_cond_signal(&queue_cond);
   pthread_mutex_unlock(&queue_mutex); 
}

bool parallel_scheduler::remove_task(Task& task) {
    pthread_mutex_lock(&queue_mutex);
    
    while (tasks.empty() && !shutdown) {
        pthread_cond_wait(&queue_cond, &queue_mutex);
    }
    
    if (shutdown && tasks.empty()) {
        pthread_mutex_unlock(&queue_mutex);
        return false;
    }
    
    task = tasks.front();
    tasks.pop();
    
    pthread_mutex_unlock(&queue_mutex);
    return true;
}

void* parallel_scheduler::work_thread(void* arg) {
    parallel_scheduler* scheduler = (parallel_scheduler*)arg;

    while (true) {
        Task task;

        if (!scheduler->remove_task(task)) {
            break;
        }

        task.function(task.argument);
    }

    return nullptr;
}
