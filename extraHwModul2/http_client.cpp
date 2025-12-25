#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
   const char* host = "httpforever.com";

   hostent* server = gethostbyname(host);
   if (server == nullptr) {
       std::cerr << "failed to get IP - adress\n";
       exit(1);
   }

   int sock = socket(AF_INET, SOCK_STREAM, 0);
   if (sock < 0) {
       std::cerr << "faled to create socket\n";
       exit(1);
   }

   sockaddr_in addr{};
   addr.sin_family = AF_INET;
   addr.sin_port = htons(80);

   std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);

   if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
       std::cerr << "failed to connect\n";
       exit(1);
   }

   const char* request = 
	   "GET / HTTP/1.1\r\n"
	   "Host: httpforevor.com\r\n"
	   "Connection: close\r\n"
	   "\r\n";

	   	
   send(sock, request, strlen(request), 0);
   int fd = open("httpforevor.html", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   if (fd  < 0) {
       std::cerr << "faiiled to open\n";
       exit(1);
   }   

   char buffer[4096];
   int bytes;

   while((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        write(fd, buffer, bytes);
    }

   close(fd);
   close(sock);

   std::cout << "saved to httpforevor.html\n";
   return 0;

}
