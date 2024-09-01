#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <cstring>
#include <limits>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#include "socket.h"
#include "admin.h"
#include "chef.h"
#include "employee.h"

class Client
{
public:
    Client(SocketConnection &client) : client(client) {}
    void start();
    void adminDisplayScreen();
    void chefDisplayScreen();
    void employeeDisplayScreen();
    bool authenticateUser(const std::string &userId, const std::string &password, const std::string &role);
    void sendRequestToServer(const std::string &request);
    void sendRequestToServerToViewMenuItem(const std::string &request);
    void sendFeedback();
    void sendRequestToServerToRollOutMenu(const std::string &request);
    void sendRequestToServerToViewRolledOutMenu(const std::string &request);
    void getRecommendations();
    void sendRequestToAddNotification(const std::string &notificationMessage);
    void sendRequestToViewNotification(const std::string &userId);
    void sendRequestToDeleteNotifications(const std::string &userId);
    void sendRequestToViewVotes(const std::string &request);
    void sendRequestToDiscardMenuItem();
    void sendRequestToDeleteDiscardMenu();
    void performActionForDiscardMenu();
;
private:
    SocketConnection client;
};
