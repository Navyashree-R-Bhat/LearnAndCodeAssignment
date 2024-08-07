#include "client.h"

void Client::sendRequestToServer(const std::string& request)
{
    send(client.clientSocket, request.c_str(), request.size(), 0);

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
    std::cout<<"Response: "<<response<<std::endl;
}

void Client::sendRequestToServer(const std::string& request)
{
    send(client.clientSocket, request.c_str(), request.size(), 0);

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
    std::cout<<"Response: "<<response<<std::endl;
}

void Client::start()
{
    while (1)
    {
        int choice;
        std::cout << "************CAFETERIA************" << std::endl;
        std::cout << "Users: " << std::endl;
        std::cout << "1.ADMIN\n2.CHEF\n3.EMPLOYEE" << std::endl;
        std::cout << "Enter the choice to login: ";
        std::cin >> choice;

        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        }
        else
        {
            switch (choice)
            {
            case 1:
                // admin
                adminDisplayScreen();
                break;
            case 2:
                // Chef
                break;
            case 3:
                // Employee
                break;
            default:
                std::cout << "You entered an invalid choice." << std::endl;
            }
        }
    }
}

bool Client::authenticateUser(const std::string &userId, const std::string &password, const std::string &role)
{
    bool status = false;
    std::string request = "VALIDATE:" + userId + ":" + password + ":" + role;
    std::cout << "Request: " << request << std::endl;
    send(client.clientSocket, request.c_str(), request.length(), 0);
    std::cout << "Authentication request sent\n";
    const int bufferSize = 1024;
    char buffer[bufferSize] = {0};
    read(client.clientSocket, buffer, sizeof(buffer));
    std::cout << "Response from server: " << buffer << std::endl;
    if (!strcmp(buffer, "success"))
    {
        status = true;
    }
    return status;
}

void Client::adminDisplayScreen()
{
    // admin
    std::string userId, password;
    std::cout << "Enter user id: ";
    std::cin >> userId;
    std::cout << "\nEnter password: ";
    std::cin >> password;
    // std::cout << "AuthenticateUser: " << authenticateUser(userId, password);
    if(authenticateUser(userId, password, "admin"))
    {
        Admin admin(userId, "ADMIN", password);
        while(true)
        {
            int choice;
            std::cout<<"\nWelcome to Admin page"<<std::endl;
            std::cout<<"Operations"<<std::endl;
            std::cout<<"1.Add User"<<std::endl;
            std::cout<<"2.Add Food Item"<<std::endl;
            std::cout<<"3.View Menu"<<std::endl;
            std::cout<<"4.Delete User"<<std::endl;
            std::cout<<"5.Delete Food Item"<<std::endl;
            std::cout<<"6.Log out"<<std::endl;
            std::cout<<"Enter your choice: ";
            std::cin>>choice;
            if (std::cin.fail()) 
            {
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
                continue;
            } 
            else 
            {
                std::string request;
                switch(choice)
                {
                    case 1:
                        request = admin.requestToAddEmployees();
                        sendRequestToServer(request);
                        break;
                    case 2:
                        // request = ;
                        break;
                    case 3:
                        //
                        break;
                    case 4:
                        request = admin.requestToDeleteEmployee();
                        sendRequestToServer(request);
                        break;
                    case 5:
                        //
                        break;
                    case 6:
                        //
                        break;
                    default:
                        std::cout << "You entered an invalid choice." << std::endl;
                }
            }
        }
    }
}