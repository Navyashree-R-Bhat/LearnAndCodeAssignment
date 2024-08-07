#pragma once

#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <cstring>
#include <thread>

#include "dbConnection.h"
#include "employee.h"
#include "Utilities.h"

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
    bool validateUser(const std::string& userId, const std::string& password, const std::string &role);
    bool addEmployeeToDatabase(Employee& employee);
    bool deleteEmployeeFromDatabase(const std::string& userId);
    // ~SocketConnection();
};
