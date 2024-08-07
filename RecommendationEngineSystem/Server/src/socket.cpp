#include "socket.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

SocketConnection::SocketConnection(int port, DatabaseConnection *database) : port(port), database(database), serverSocket(0), clientSocket(0), addressLength(sizeof(serverAddress))
{
    memset(&serverAddress, 0, sizeof(serverAddress));
    createSocket();
    bindingSocket();
    initializeDatabase();
    // database = new DatabaseConnection();
    // database->connectDB();
    // serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // serverAddress.sin_family = AF_INET;
    // serverAddress.sin_port = htons(8080);
    // serverAddress.sin_addr.s_addr = INADDR_ANY;
}

void SocketConnection::createSocket()
{
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}
void SocketConnection::waitForExit(SocketConnection &server)
{
    cin.get();
    server.stopServer();
}

void SocketConnection::initializeDatabase()
{
    try
    {
        std::unique_ptr<sql::Statement> statement(database->getConnection()->createStatement());

        statement->execute("CREATE TABLE IF NOT EXISTS Users("
                           "user_id VARCHAR(10) PRIMARY KEY,"
                           "username VARCHAR(255) NOT NULL,"
                           "password VARCHAR(255) NOT NULL,"
                           "role ENUM('admin', 'chef', 'employee') NOT NULL)");
        statement->execute("CREATE TABLE IF NOT EXISTS MenuItems("
                           "item_id INT  PRIMARY KEY,"
                           "item_name VARCHAR(255) NOT NULL,"
                           "price DECIMAL(10, 2) NOT NULL,"
                           "availability_status BOOLEAN NOT NULL)");
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL error: " << e.what() << std::endl;
        throw;
    }
}

void SocketConnection::bindingSocket()
{
    int option = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)))
    {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

void SocketConnection::listeningToSocket()
{
    if (listen(serverSocket, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

void SocketConnection::accpetingConnection()
{
    if ((clientSocket = accept(serverSocket, (sockaddr *)&serverAddress, (socklen_t *)&addressLength)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
}

void SocketConnection::receiveMessage(int clientSocket)
{
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from client: " << buffer << endl;
    send(clientSocket, "Hello from server!", 18, 0);
}

void SocketConnection::stopServer()
{
    stopFlag = true;
    close(clientSocket);
    close(serverSocket);
}

void SocketConnection::handleRequest()
{
    while (true)
    {
        char buffer[bufferSize] = {0};
        read(clientSocket, buffer, bufferSize);
        std::string request(buffer);
        std::cout << "Request: " << request << std::endl;

        std::istringstream ss(request);
        std::string command;
        std::getline(ss, command, ':');
        std::cout << "Command received " << command << std::endl;

        std::string response;

        if (command == "VALIDATE")
        {
            std::cout << "Validate" << std::endl;
            std::string userId, password, role;
            std::getline(ss, userId, ':');
            std::getline(ss, password, ':');
            std::getline(ss, role, ':');
            std::cout << userId << "  " << password << std::endl;
            if (validateUser(userId, password, role))
            {
                response = "success";
            }
            else
            {
                response = "failed";
            }
        }
        else if (command == "REGISTER_EMPLOYEE")
        {
            std::cout<<"Registering employee"<<std::endl;
            std::string employeeName;
            std::string employeeId;
            std::string password;
            std::string role;
            std::getline(ss, employeeId, ':');
            std::getline(ss, employeeName, ':');
            std::getline(ss, password, ':');
            std::getline(ss, role, ':');

            std::cout<<employeeId<<":"<<employeeName<<":"<<password<<std::endl;

            Employee employeeDetails(employeeId, employeeName, password, role);
            if(addEmployeeToDatabase(employeeDetails))
            {
                response = "Employee added successfully";
            }
            else
            {
                response = "Failed to add new employee";
            }
        }
        else if (command =="DELETE_USER")
        {
            std::cout<<"Deleting user"<<std::endl;
            std::string userId;
            std::getline(ss, userId, ':');
            if (deleteEmployeeFromDatabase(userId))
            {
                response = "User deleted successfully";
            }
            else
            {
                response = "Failed to delete user";
            }
        }
        send(clientSocket, response.c_str(), response.length(), 0);
        std::cout << "Response sent: " << response << std::endl;
        memset(buffer, 0, bufferSize);
    }
}

void SocketConnection::run()
{
    listeningToSocket();
    while (true)
    {
        accpetingConnection();
        std::thread clientRequestThread(&SocketConnection::handleRequest, this);
        clientRequestThread.detach();
    }
}

bool SocketConnection::validateUser(const std::string &userId, const std::string &password, const std::string &role)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement("SELECT password, role FROM Users WHERE user_id = ?"));
        pStatement->setString(1, userId);

        std::unique_ptr<sql::ResultSet> result(pStatement->executeQuery());
        if (result->next())
        {
            std::string storedPassword = result->getString("password");
            auto storedRole = result->getInt("role");
            std::cout << "Stored Password" << storedPassword << std::endl;
            std::cout << "Stored Role" << storedRole << std::endl;
            return storedPassword == password && storedRole == Utilities::getRoleEnum(role);
        }
    }
    catch (sql::SQLException &exception)
    {
        std::cerr << "MySQL error: " << exception.what() << std::endl;
    }
    return false;
}

bool SocketConnection::addEmployeeToDatabase(Employee& employee)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement("INSERT INTO Users (user_id, username, password, role) VALUES (?, ?, ?, ?)"));
        pStatement->setString(1,employee.getEmployeeId());
        pStatement->setString(2,employee.getEmployeeName());
        pStatement->setString(3,employee.getPassword());
        pStatement->setString(4,employee.getRole());
        pStatement->execute();
        return true;
    }
    catch (sql::SQLException& exception)
    {
        std::cerr<<"MySQL error: "<<exception.what()<<std::endl;
    }
    return false;
}

bool SocketConnection::deleteEmployeeFromDatabase(const std::string& userId)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement("DELETE FROM Users WHERE user_id = ?"));
        pStatement->setString(1, userId);
        pStatement->execute();
        return true;
    }
    catch(sql::SQLException& exception)
    {
        std::cerr<<"MySQL error: "<<exception.what()<<std::endl;
    }
    return false;
}
// int main()
// {
//     // DatabaseConnection database;
//     // SocketConnection server(8080, &database);
//     // server.bindingSocket();
//     server.listeningToSocket();
//     std::thread clientRequestThread(&SocketConnection::handleRequest, this);
//     server.accpetingConnection();
//     // server.receiveMessage();
//     clientRequestThread.join();
// 	return 0;
// }
