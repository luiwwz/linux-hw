#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        const char* msg = "Usage: ./read-file <file_name>\n";
        write(STDERR_FILENO, msg, 33);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        const char* msg = "Error: failed to open file\n";
        write(STDERR_FILENO, msg, 28);
        return 1;
    }

    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    close(fd);
    return 0;
}
