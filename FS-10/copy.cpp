#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>

void writing(int source_fd, int dest_fd, int count) {
     char buff[4096];
     off_t diff;
     while(count > 0) {
	diff  = count < 4096 ? count : 4096;     
     	off_t reading = read(source_fd, buff, diff);
	if (reading == -1) {
	    std::cerr << "failed to read from source file\n";
	    exit(1);
	}

	if (reading == 0) {
	    break;
	}

	off_t written = write(dest_fd, buff, reading);
	if (written == -1) {
	    std::cerr << "failed to write in destination fail\n";
	    exit(1);
	}

	count -= diff; 
     }	     
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Invalid arguments count.\n";
	exit(1);
    }	

    const char* source_path = argv[1];
    const char* dest_path = argv[2];

    int source_fd = open(source_path, O_RDONLY);
    if (source_fd < 0) {
        std::cerr << "failed to open source file.\n";
	exit(1);
    }

    struct stat source_st;
    if (fstat(source_fd, &source_st) == -1) {
        std::cerr << "failed to stat source file.\n";
	exit(1);
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        std::cerr << "failed to open destinaton file.\n";
	exit(1);
    }

    off_t logical_size = source_st.st_size;
    off_t physical_size = 0;
    off_t hole_size = 0;

    off_t  offset = 0;
    while (offset < logical_size) {
            off_t seek1 = lseek(source_fd, offset, SEEK_DATA) ;
	    if (seek1 == -1) {
	       if (errno == ENXIO) {    
	           hole_size += logical_size - offset;
                   off_t p = ftruncate(dest_fd, logical_size);
		   break;
	       }	    
	       std::cerr << "failed to seek1\n";
	       exit(1);
	    }

	    if (seek1 > offset) {    
	        hole_size += seek1 - offset;
		off_t seek_dest = lseek(dest_fd, seek1, SEEK_SET);
		if (seek_dest == -1) {	
		    std::cerr << "failed to seek data in destination file\n";
		    exit(1);
		}

		offset = seek1;
	     } 
	        
	    off_t seek2 = lseek(source_fd, offset, SEEK_HOLE);
	    if (seek2 == -1) { 
	        std::cerr << "failed to seek2\n";
	        exit(1);
	     }
		
	    off_t seek_return = lseek(source_fd, offset, SEEK_SET);
	    if (seek_return == -1) {
	        std::cerr << "failed to return seek\n";
   	        exit(1);		    
	     }
   
	    off_t diff = seek2 - offset;
            writing(source_fd, dest_fd, diff);
            physical_size += diff;

            offset = seek2;

    }

    close(source_fd);
    close(dest_fd);

    std::cout << "Successfully copied " << logical_size << " bytes " 
	    <<  "(data: " << physical_size << ", hole: " << hole_size << ")\n";

    return 0;

}
