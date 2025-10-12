#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>

void initialize(int argc, char** argv)
{
     if (argc != 2) {
         std::cerr << "Invalid argumetns count.\n";
	 exit(1);
     }

     int fd = open(argv[1], O_RDONLY);
     if (fd < 0) {
         std::cerr << "failed to open.\n";
	 exit(1);
     }

     int db  = dup2(fd, 0);
     if (db < 0) {
         std::cerr << "failed to dub.\n";
	 exit(1);
     }
     
}
 
int main(int argc, char** argv)
{
  initialize(argc, argv);
   
  std::string input;
  std::cin >> input;
  
  std::reverse(input.begin(), input.end());
  
  std::cout << input;
 
  return 0;
}
