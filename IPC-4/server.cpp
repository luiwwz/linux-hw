#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

struct Client {
    int socket;
    std::string name;
};

std::vector<Client> clients;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const std::string& message, int sender_fd) {
    pthread_mutex_lock(&clients_mutex);
    for (auto& c : clients) {
        if (c.socket != sender_fd) {
            send(c.socket, message.c_str(), message.length(), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void* handle_client(void* arg) {
    int client_socket = *((int*)arg);
    delete (int*)arg;

    char buffer[1024];
    int bytes = recv(client_socket, buffer, sizeof(buffer)-1, 0);
    if (bytes <= 0) { close(client_socket); return nullptr; }
    buffer[bytes] = '\0';
    std::string name(buffer);

    pthread_mutex_lock(&clients_mutex);
    clients.push_back({client_socket, name});
    pthread_mutex_unlock(&clients_mutex);

    broadcast("--- " + name + " has joined the chat ---\n", client_socket);

    while (true) {
        bytes = recv(client_socket, buffer, sizeof(buffer)-1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        std::string msg(buffer);

        if (msg == "/exit") break;

        if (msg == "/list") {
            std::string list = "Connected users: ";
            pthread_mutex_lock(&clients_mutex);
            for (auto& c : clients) list += c.name + " ";
            pthread_mutex_unlock(&clients_mutex);
            list += "\n";
            send(client_socket, list.c_str(), list.length(), 0);
        } else {
            broadcast("[" + name + "]: " + msg + "\n", client_socket);
        }
    }

    pthread_mutex_lock(&clients_mutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(), 
                  [client_socket](const Client& c){ return c.socket == client_socket; }), clients.end());
    pthread_mutex_unlock(&clients_mutex);

    broadcast("--- " + name + " has left the chat ---\n", -1);
    close(client_socket);
    return nullptr;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{AF_INET, htons(8080), INADDR_ANY};

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed"); return 1;
    }
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed"); return 1;
    }

    std::cout << "Server started on port 8080..." << std::endl;

    while (true) {
        int* client_sock = new int(accept(server_fd, nullptr, nullptr));
        if (*client_sock < 0) { delete client_sock; continue; }

        pthread_t tid;
        pthread_create(&tid, nullptr, handle_client, client_sock);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
