#include "socket.h"

#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

using namespace std; 

SocketConnection::SocketConnection(int port, DatabaseConnection *database): port(port), database(database), serverSocket(0), clientSocket(0), addressLength(sizeof(serverAddress))
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
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}
void SocketConnection::waitForExit(SocketConnection& server) 
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
                                     "item_id INT AUTO_INCREMENT PRIMARY KEY,"
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
    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)))
    {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))<0)
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
    if((clientSocket = accept(serverSocket, (sockaddr*)&serverAddress, (socklen_t *)&addressLength))<0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
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
    close(clientSocket);
    close(serverSocket);
}

void SocketConnection::handleRequest()
{
    char buffer[bufferSize] = {0};
    read(clientSocket, buffer, bufferSize);
    std::string request(buffer);
    std::cout<<"Request: "<<request<<std::endl;
    
    std::istringstream ss(request);
    std::string command;
    std::getline(ss, command, ':');
    std::cout << "Command received " << command << std::endl;

    std::string response;
}

void SocketConnection::run()
{
    listeningToSocket();
    while(true)
    {
        accpetingConnection();
        std::thread clientRequestThread(&SocketConnection::handleRequest, this);
        clientRequestThread.detach();
    }
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
