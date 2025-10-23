#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        const char* msg = "Usage: ./simple-copy <source> <destination>\n";
        ssize_t check = write(STDERR_FILENO, msg, strlen(msg));
        if (check < 0) {
            std::cerr << "failed to write to stderr\n";
            exit(1);
        }
        return 1;
    }

    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0) {
        const char* msg = "Error: cannot open source file\n";
        ssize_t check = write(STDERR_FILENO, msg, strlen(msg));
        if (check < 0) {
            std::cerr << "failed to write to stderr\n";
            exit(1);
        }
        return 1;
    }

    int fd_dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst < 0) {
        const char* msg = "Error: cannot open/create destination file\n";
        ssize_t check = write(STDERR_FILENO, msg, strlen(msg));
        if (check < 0) {
            std::cerr << "failed to write to stderr\n";
            close(fd_src);
            exit(1);
        }
        close(fd_src);
        return 1;
    }

    char buffer[1024];
    ssize_t n;
    while ((n = read(fd_src, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(fd_dst, buffer, n);
        if (bytes_written < 0) {
            std::cerr << "failed to write to destination file\n";
            close(fd_src);
            close(fd_dst);
            exit(1);
        }

        if (bytes_written != n) {
            std::cerr << "not all bytes written to destination file\n";
            close(fd_src);
            close(fd_dst);
            exit(1);
        }
    }

    if (n < 0) {
        std::cerr << "failed to read from source file\n";
        close(fd_src);
        close(fd_dst);
        exit(1);
    }

    close(fd_src);
    close(fd_dst);
    return 0;
}

