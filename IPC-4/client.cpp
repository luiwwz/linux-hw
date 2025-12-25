#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void receive_messages(int sock) {
    char buffer[1024];
    while (true) {
        int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (bytes <= 0) {
            std::cout << "\nConnection to server lost." << std::endl;
            exit(0);
        }
        buffer[bytes] = '\0';
        std::cout << buffer << std::flush;
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{AF_INET, htons(8080)};
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connect failed"); return 1;
    }

    std::cout << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    send(sock, name.c_str(), name.length(), 0);

    std::thread(receive_messages, sock).detach();

    std::string msg;
    while (true) {
        std::getline(std::cin, msg);
        send(sock, msg.c_str(), msg.length(), 0);
        if (msg == "/exit") break;
    }

    close(sock);
    return 0;
}
