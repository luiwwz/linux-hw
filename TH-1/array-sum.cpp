#include <iostream>
#include <vector>
#include <pthread.h>
#include <sys/time.h>
#include <random>

struct ThreadData {
       std::vector<int>* arr;
       size_t sum = 0;
       size_t start;
       size_t end;
};

void* count_sum(void* arg) {
      ThreadData* obj = (ThreadData*)arg;
      size_t subsum = 0;
      for (size_t i = (*obj).start; i < (*obj).end; i++) {
	   subsum += (*(*obj).arr)[i];
      }	

      (*obj).sum = subsum;
      return nullptr;
}

size_t count_with_threads(ThreadData* th, const size_t M) {
       size_t result = 0;		
       std::vector<pthread_t> threads(M);
       std::vector<ThreadData> thread_data(M);

       size_t chunk = th->arr->size() / M;

       for (size_t i = 0; i < M; i++) {  
           ThreadData tmp;
	   tmp.arr = th->arr;
	   tmp.start = i * chunk;
	   tmp.end = (i + 1) * chunk;
	   thread_data[i] = tmp;
       }       

       thread_data[M - 1].end += th->arr->size() - (chunk * M); 

       for (size_t i = 0; i < M; i++) {
            size_t check = pthread_create(&threads[i], nullptr, count_sum, &thread_data[i]);
	    if (check != 0) {
	        std::cerr << "failed to creat thread\n";
		exit(1);
	    }
       }

       for (size_t i = 0; i < M; i++) {
            size_t check = pthread_join(threads[i], nullptr);
	    if (check != 0) {
	        std::cerr << "failed to join\n";
		exit(1);
	    }

	    result += thread_data[i].sum;
       }

       return result;
}


int main(int argc, char* argv[]) {
     if (argc != 3) {
         std::cerr << "Invalid arguments count\n";
	 exit(1);
     }

     size_t N = std::atoi(argv[1]);
     size_t M = std::atoi(argv[2]);

     if (N < 1000000) {
         std::cerr << "N must be > 1000000\n";
	 exit(1);
     }

     std::mt19937 gen(time(nullptr));
     std::uniform_int_distribution<int> dist(0, 1000);
     std::vector<int> array(N);
     for (size_t i = 0; i < N; i++) {
          array[i] = dist(gen);
     }

     struct timeval time;
     ThreadData threads;
     threads.arr = &array;
     threads.start = 0;
     threads.end = N;

     gettimeofday(&time, nullptr);
     double begin = time.tv_sec + time.tv_usec / 1e6;

     count_sum(&threads);

     gettimeofday(&time, nullptr);
     double end = time.tv_sec + time.tv_usec / 1e6;

     std::cout <<"Sum: " << threads.sum << ", Time spent without threads: " << end - begin << '\n';

     gettimeofday(&time, nullptr);
     begin = time.tv_sec + time.tv_usec / 1e6;

     size_t res = count_with_threads(&threads, M);
    
     gettimeofday(&time, nullptr);
     end = time.tv_sec + time.tv_usec / 1e6;

     std::cout << "Sum: " << res << ", Time spent with threads: " << end - begin << '\n';

     return 0;	     
}
