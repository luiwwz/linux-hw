#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctime>
#include <cstring>

void do_command(char** argv) {
     time_t start = time(nullptr);

     pid_t pid = fork();
     if (pid < 0) {
         perror("fork failed");
	 exit(1);
     }

     if (pid == 0) {
         execvp(argv[1], &argv[1]);

	 perror("execvo failed");
	 exit(1);
     } else {
       int status;
       if (waitpid(pid, &status, 0) == -1) {
           perror("waitpid error");
	   exit(1);
         }

       time_t end = time(nullptr);
       double dur = difftime(end, start);

       if (WIFEXITED(status)) {
           std::cout << "Command completed with " << status << " exit code and took " 
		   << dur << " time\n";
       } else {
           exit(1);
       }
   }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Invalid input\n";
        exit(1);	
    }

    char** args = new char*[argc+1];
    for (int i = 0; i < argc; i++) {
         args[i] = new char[std::strlen(argv[i])];
	 std::strcpy(args[i], argv[i]);
    }
    args[argc] = nullptr;

    do_command(args);

    delete[] args;
    return 0;
}
