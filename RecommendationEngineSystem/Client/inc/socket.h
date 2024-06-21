#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

#define PORT 8080 

class SocketConnection
{
public:
    int clientSocket;
    int serverSocket;
    sockaddr_in serverAddress; 

    SocketConnection();
    void connectToServer();
    void receiveMessage();
    void sendMessage(const std::string& message);
    ~SocketConnection();
};