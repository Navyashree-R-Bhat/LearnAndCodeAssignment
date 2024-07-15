#pragma once

#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <cstring>
#include <thread>

#include "dbConnection.h"

#define PORT 8080 

class SocketConnection
{
public:
    int port;
    int addressLength;
    const int bufferSize = 1024;
    int serverSocket;
    int clientSocket;
    sockaddr_in serverAddress;
    std::atomic<bool> stopFlag = false;
    DatabaseConnection *database;

    SocketConnection(int port, DatabaseConnection *database);
    void bindingSocket();
    void createSocket();
    void handleRequest();
    void listeningToSocket();
    void accpetingConnection();
    void receiveMessage(int clientSocket);
    void sendMessage(const std::string& message);
    static void waitForExit(SocketConnection& server);
    void run();
    void initializeDatabase();
    void stopServer();
    // ~SocketConnection();
};
