#include "socket.h"

#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

SocketConnection::SocketConnection(int port) : port(port), clientSocket(0)
{
    memset(&address, 0, sizeof(address)); 
    createSocket();
    connectToServer();
}

void SocketConnection::createSocket()
{
    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void SocketConnection::connectToServer()
{
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
}

void SocketConnection::sendMessage(const std::string& message)
{
    send(clientSocket, message.c_str(), message.size(), 0);
}

void SocketConnection::receiveMessage()
{
    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from server: " << buffer << std::endl;
}

SocketConnection::~SocketConnection()
{
    close(clientSocket);
}
