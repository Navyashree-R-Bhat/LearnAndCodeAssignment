#include "socket.h"

#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

SocketConnection::SocketConnection()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(8080); 
	serverAddress.sin_addr.s_addr = INADDR_ANY;
}

void SocketConnection::connectToServer()
{
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
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

int main() 
{
    SocketConnection client;
    client.connectToServer();
    client.sendMessage("Hello, server!");
    client.receiveMessage();
	return 0; 
}
