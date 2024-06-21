#include "socket.h"

#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

using namespace std; 

SocketConnection::SocketConnection()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(8080); 
	serverAddress.sin_addr.s_addr = INADDR_ANY;
}

void SocketConnection::waitForExit(SocketConnection& server) 
{
    cin.get();
    server.stopServer();
}


void SocketConnection::bindingSocket()
{
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
}

void SocketConnection::listeningToSocket()
{
    listen(serverSocket, 5);
}

void SocketConnection::accpetingConnection()
{
    while(!stopFlag)
    {
        clientSocket = accept(serverSocket, nullptr, nullptr);
        std::thread runClientRequest(&SocketConnection::receiveMessage, this, clientSocket);
        runClientRequest.detach();
    }
}

void SocketConnection::receiveMessage(int clientSocket)
{
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);
        cout << "Message from client: " << buffer << endl;
        send(clientSocket, "Hello from server!", 18, 0);
}

void SocketConnection::stopServer() {
    stopFlag = true;
    close(serverSocket);
}

int main() 
{
    SocketConnection server;
    server.bindingSocket();
    server.listeningToSocket();
    std::thread exitThread(SocketConnection::waitForExit, std::ref(server));
    server.accpetingConnection();
    // server.receiveMessage();
    exitThread.join();
	return 0; 
}
