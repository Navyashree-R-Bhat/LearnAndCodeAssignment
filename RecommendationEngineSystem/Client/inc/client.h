#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <limits>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "socket.h"
#include "admin.h"

class Client
{
public:
    Client(SocketConnection &client) : client(client) {}
    void start();
    void adminDisplayScreen();
    void chefDisplayScreen();
    void employeeDisplayScreen();
    bool authenticateUser(const std::string &userId, const std::string &password);
    void sendRequestToServer(const std::string &request);

private:
    SocketConnection client;
};