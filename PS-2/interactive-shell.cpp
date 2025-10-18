#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

std::vector<std::string> split(const std::string& input) {
	std::vector<std::string> tokens;
	std::string token;
	bool is_word = false;

	for (auto& c : input) {
	     if (c == ' ') {
	         if (is_word) {
		     tokens.push_back(token);
		     token.clear();
		     is_word = false;
		 }
	     } else {
	       token += c;
	       is_word = true;
	     }
	}

	if (!token.empty()) {
	    tokens.push_back(token);
	}

	return tokens;
}


void do_command(const std::string& command_line) {
	std::vector<std::string> tokens = split(command_line);
	if (tokens.empty()) {
	    return;
	 }

	bool silent = false;

	if (tokens[0] == "exit") {
	    exit(0);
	}

	if (tokens[0] == "silent") {
	    silent = true;
    	    if (tokens.size() < 2) {
	        std::cerr << "error\n";
		return;
	    }		    
	}

	std::vector<char*> c_args;
	for (const auto& arg : tokens) {
	     c_args.push_back(const_cast<char*>(arg.c_str()));
	}
	c_args.push_back(nullptr);

	pid_t pid = fork();
	if (pid == -1) {
	    std::cerr << "failed to fork\n";
	    exit(1);
	}

	if (pid == 0) {
	    std::string new_path = "./:" + std::string(getenv("PATH"));
	    setenv("PATH", new_path.c_str(), 1);

	    if (silent) {
		c_args.erase(c_args.begin());    
	 	std::string logfile = std::to_string(getpid()) + ".log";
	    	int fd = open(logfile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1) {
		    dup2(fd, STDOUT_FILENO);
		    dup2(fd, STDERR_FILENO);
		    close(fd);
		} else {
		  std::cerr << "failed to open\n";
		  exit(1);
		}
	    }

	    if (execvp(c_args[0], c_args.data()) == -1) {
	        std::cerr << "execvp failed\n";
		exit(1);
	    } 
              
	}

	if (pid > 0) {
	    int status;
	    int check = waitpid(pid, &status, 0);
	    if (check == -1) {
	        std::cerr << "waitpid failed\n";
		exit(1);
	    }

	    if (!WIFEXITED(status)) {
		    std::cerr << "child execution was failed\n";
	     }
	}
}

int main() {
    std::string line;

    while (true) {
        std::cout << "$ ";
	std::getline(std::cin, line);
	if (!line.empty()) {
	     do_command(line);
	}
    }

    return 0;
}
