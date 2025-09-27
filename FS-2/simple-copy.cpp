#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        const char* msg = "Usage: ./simple-copy <source> <destination>\n";
        write(2, msg, 44); 
        return 1;
    }

    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0) {
        const char* msg = "Error: cannot open source file\n";
        write(2, msg, 33);
        return 1;
    }

    int fd_dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst < 0) {
        const char* msg = "Error: cannot open/create destination file\n";
        write(2, msg, 44);
        close(fd_src);
        return 1;
    }

    char buffer[1024];
    ssize_t n;
    while ((n = read(fd_src, buffer, sizeof(buffer))) > 0) {
        write(fd_dst, buffer, n);
    }

    close(fd_src);
    close(fd_dst);
    return 0;
}
