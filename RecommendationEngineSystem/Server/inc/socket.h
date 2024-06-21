#pragma once

#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#define PORT 8080 

class SocketConnection
{
public:
    int serverSocket;
    int clientSocket;
    sockaddr_in serverAddress;
    std::atomic<bool> stopFlag = false;

    SocketConnection();
    void bindingSocket();
    void listeningToSocket();
    void accpetingConnection();
    void receiveMessage(int clientSocket);
    void sendMessage(const std::string& message);
    static void waitForExit(SocketConnection& server);
    void stopServer();
    // ~SocketConnection();
};
