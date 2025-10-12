#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid arguments count.\n";
	exit(1);
    } 

    const char* path = argv[1];

    int fd = open(path, O_WRONLY | O_TRUNC);
    if (fd < 0) {
        std::cerr << "failed to open.\n";
	exit(1);
    }

    int fd2 = dup(fd);
    if (fd2 < 0) {
        std::cerr << "failed to duplicate.\n";
        close(fd);
        exit(1);	
    }

    const char*  first_line = "first line\n";
    int written = write(fd, first_line, strlen(first_line));
    if (written < 0) {
        std::cerr << "failed to write.\n";
	close(fd);
	close(fd2);
	exit(1);
    } 

    const char* second_line = "second line\n";
    written = write(fd2, second_line, strlen(second_line));
    if (written < 0) {
        std::cerr << "failed to write.\n";
	close(fd);
	close(fd2);
	exit(1);
    }

    close(fd);
    close(fd2);

    return 0;
}

