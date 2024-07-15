#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

#define PORT 8080 

class SocketConnection
{
public:
    int clientSocket;
    int serverSocket;
    int port;
    std::string serverAddress;
    sockaddr_in address; 

    SocketConnection(int port);
    void createSocket();
    void connectToServer();
    void receiveMessage();
    void sendMessage(const std::string& message);
    ~SocketConnection();
};