#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
<<<<<<< HEAD

std::vector<std::string> split(const std::string& input) {
	std::vector<std::string> tokens;
	std::string token;
	bool is_word = false;
=======
>>>>>>> 753388b (adding PS-2)

struct Command {
    std::string cmd;
    std::string op;
};

std::vector<Command> split_commands(const std::string& str) {
    std::vector<Command> commands;
    std::string current;
    std::string last_op;
    for (size_t i = 0; i < str.size();) {
        if (i + 1 < str.size() && str[i] == '&' && str[i+1] == '&') {
            commands.push_back({current, last_op});
            current.clear();
            last_op = "&&";
            i += 2;
        } else if (i + 1 < str.size() && str[i] == '|' && str[i+1] == '|') {
            commands.push_back({current, last_op});
            current.clear();
            last_op = "||";
            i += 2;
        } else if (str[i] == ';') {
            commands.push_back({current, last_op});
            current.clear();
            last_op.clear();
            i++;
        } else {
            current += str[i];
	    i++;
        }
    }
    if (!current.empty()) {
        commands.push_back({current, last_op});
    }

    return commands;
}

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

int do_command(const std::string& command_line) {
    std::vector<std::string> tokens = split(command_line);
    if (tokens.empty()) {
        return 0;
    }

    bool silent = false;
    int redirect_fd = -1;
    bool append = false;

    if (tokens[0] == "exit") {
        exit(0);
    }

    if (tokens[0] == "silent") {
        silent = true;
        tokens.erase(tokens.begin());
        if (tokens.empty()) {
            std::cerr << "error: no command after silent\n";
            return 1;
        }
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == ">" || tokens[i] == ">>") {
            append = (tokens[i] == ">>");
            if (i + 1 >= tokens.size()) {
                std::cerr << "error: no file after redirection\n";
                return 1;
            }

            std::string filename = tokens[i + 1];
            int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
            redirect_fd = open(filename.c_str(), flags, 0644);
            if (redirect_fd == -1) {
	        std::cerr << "failed to open\n";
                return 1;
            }

            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            break;
        }
    }

    std::vector<char*> c_args;
    for (auto& arg : tokens) {
         c_args.push_back(const_cast<char*>(arg.c_str())); 
    }
    c_args.push_back(nullptr);

    pid_t pid = fork();
    if (pid == -1) {
	std::cerr << "failed to fork\n";
        return 1;
    }

    if (pid == 0) {
        std::string new_path = "./:" + std::string(getenv("PATH"));
        setenv("PATH", new_path.c_str(), 1);

        if (silent) {
            std::string logfile = std::to_string(getpid()) + ".log";
            int fd = open(logfile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd != -1) {
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }
        }

        if (redirect_fd != -1) {
            dup2(redirect_fd, STDOUT_FILENO);
            dup2(redirect_fd, STDERR_FILENO);
            close(redirect_fd);
        }

        if (execvp(c_args[0], c_args.data()) == -1) {
	    std::cerr << "failed to exec\n";
            exit(1);
	}
    }

    int status;
    waitpid(pid, &status, 0);
    if (redirect_fd != -1) {
	    close(redirect_fd);
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int main() {
    std::string input;
    while (true) {
        std::cout << "$ ";
        if (!std::getline(std::cin, input)) {
	    break;
	}
        if (input.empty()) {
	    continue;
	}

        auto commands = split_commands(input);
        int last_status = 0;

        for (auto& c : commands) {
             bool run = true;
             if (c.op == "&&" && last_status != 0) {
	         run = false;
	     }
             if (c.op == "||" && last_status == 0) {
	         run = false;
	     }

             if (run) {
                last_status = do_command(c.cmd);
	     }
        }
    }

    return 0;
}
