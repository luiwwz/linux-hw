#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        const char* msg = "Usage: ./read-file <file_name>\n";
        ssize_t check = write(STDERR_FILENO, msg, strlen(msg));

        if (check < 0) {
            std::cerr << "failed to write to stderr\n";
            exit(1);
        }
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        const char* msg = "Error: failed to open file\n";
        ssize_t check = write(STDERR_FILENO, msg, strlen(msg)); 
        if (check < 0) {
            std::cerr << "failed to write to stderr\n";
            exit(1);
        }
        return 1;
    }

    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written < 0) {
            std::cerr << "failed to write to stdout\n";
            close(fd);
            exit(1);
        }
        if (bytes_written != bytes_read) {
            std::cerr << "not all bytes written to stdout\n";
            close(fd);
            exit(1);
        }
    }

    if (bytes_read < 0) {
        std::cerr << "failed to read file\n";
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}

