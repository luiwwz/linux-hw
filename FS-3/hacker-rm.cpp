#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid arguments count.\n";
        exit(1); 
    }    

    const char* path = argv[1];

    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        std::cerr << "failed to open.\n";
        exit(1); 
    }

    struct stat st;
    int check = stat(path, &st);
    if (check < 0) {
        std::cerr << "failed to copy metadata.\n";
        close(fd); 
        exit(1);
    }

    off_t size = st.st_size;
    if (size == 0) {
        int closing = close(fd);
        if (closing < 0) {
            std::cerr << "failed to close.\n";
            exit(1);
        }
        
        int unlinked = unlink(path);
        if (unlinked < 0) {
            std::cerr << "failed to unlink.\n";
            exit(1); 
        }
    }

    char buff[4096] = {'\0'};
    while (size > 0) {
        int min = size < 4096 ? size : 4096;

        int writing = write(fd, buff, min);
        if (writing < 0) {
            std::cerr << "failed to write.\n";
            close(fd); 
            exit(1);
        }

        size -= min;
    }

    int check2 = close(fd);
    if (check2 < 0) {
        std::cerr << "failed to close.\n";
        exit(1); 
    }

    int unlinked2 = unlink(path);
    if (unlinked2 < 0) {
        std::cerr << "failed to unlink.\n";
        exit(1); 
    }

    return 0;
}

