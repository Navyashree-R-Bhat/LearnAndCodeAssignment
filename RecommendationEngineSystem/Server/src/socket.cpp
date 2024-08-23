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
                           "item_id INT PRIMARY KEY,"
                           "item_name VARCHAR(255) NOT NULL,"
                           "price DECIMAL(10, 2) NOT NULL,"
                           "availability_status BOOLEAN NOT NULL)");
        statement->execute("CREATE TABLE IF NOT EXISTS Feedback("
                           "feedback_id INT AUTO_INCREMENT PRIMARY KEY, "
                           "item_id INT,"
                           "user_id VARCHAR(10),"
                           "rating INT,"
                           "comment VARCHAR(255),"
                           "FOREIGN KEY (item_id) REFERENCES MenuItems(item_id) ON DELETE CASCADE,"
                           "FOREIGN KEY (user_id) REFERENCES Users(user_id) ON DELETE CASCADE)");
        statement->execute("CREATE TABLE IF NOT EXISTS DailyMenu("
                           "item_id INT,"
                           "menu_date DATE DEFAULT (CURRENT_DATE),"
                           "meal_type ENUM('Breakfast', 'Lunch', 'Dinner'),"
                           "PRIMARY KEY (item_id, menu_date, meal_type),"
                           "FOREIGN KEY (item_id) REFERENCES MenuItems(item_id))");
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

int SocketConnection::accpetingConnection()
{
    if ((clientSocket = accept(serverSocket, (sockaddr *)&serverAddress, (socklen_t *)&addressLength)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    return clientSocket;
}

void SocketConnection::receiveMessage(int clientSocket)
{
    char buffer[4096] = {0};
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

void SocketConnection::handleRequest(int clientSocket)
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
        else if (command=="ADD_MENU_ITEM")
        {
            std::cout<<"Adding menu item"<<std::endl;
            std::string itemName, word;
            bool availabilityStatus;
            int itemId, itemPrice;
            std::getline(ss, word, ':');
            itemId = std::stoi(word);
            std::getline(ss, itemName, ':');
            std::getline(ss, word, ':');
            itemPrice = std::stoi(word);
            std::getline(ss, word, ':');
            availabilityStatus = Utilities::stringToBool(word);
            // availabilityStatus = word ? true:false;
            std::cout<<"ITEM: "<<itemId<<":"<<itemName<<":"<<itemPrice<<":"<<availabilityStatus<<std::endl;
            MenuItem menuItem(itemId, itemName, itemPrice, availabilityStatus);
            if(addMenuItemToDatabase(menuItem))
            {
                response = "Menu Item added successfully";
            }
            else
            {
                response = "Failed to add new Menu Item";
            }
        }
        else if(command == "DELETE_MENU_ITEM")
        {
            std::cout<<"Deleting menu item"<<std::endl;
            std::string word;
            std::getline(ss, word, ';');
            int itemId = std::stoi(word);
            std::cout<<"Item deleted: "<<itemId<<std::endl;
            if(deleteMenuItemFromDatabase(itemId))
            {
                response = "Menu Item deleted successfully";
            }
            else
            {
                response = "Failed to delete Menu Item";
            }
        }
        else if(command == "VIEW_MENU_ITEMS")
        {
            std::cout<<"Viewing menu"<<std::endl;
            std::string responseToBeSent;
            auto menuItems = viewMenuItem();

            for (auto &item : menuItems)
            {
                responseToBeSent += std::to_string(item.getItemId()) + ":" + item.getItemName() + ":" + std::to_string(item.getItemPrice()) + ":" + std::to_string(item.getAvailabilityStatus()) + "|";
                std::cout<<"Menu Item: "<<response<<std::endl;
            }
            if (!responseToBeSent.empty())
            {
                responseToBeSent.pop_back();
            }
            response = responseToBeSent;
        }
        else if(command == "ADD_FEEDBACK")
        {
            std::string userId, comment, token;
            int itemId, rating;
            std::getline(ss, token, ':');
            itemId = std::stoi(token);
            std::getline(ss, userId, ':');
            std::getline(ss, token, ':');
            rating = std::stoi(token);
            std::getline(ss, comment, ':');

            std::cout<<"Adding feedback: ";
            std::cout<<itemId<<" "<<userId<<" "<<rating<<" "<<comment<<std::endl;
            Feedback feedback(itemId, userId, rating, comment);

            if(addFeedbackToDatabase(feedback))
            {
                response = "Feedback stored successfully";
            }
            else
            {
                response = "Failed to store feedback";
            }
        }
        else if(command == "ROLL_OUT_MENU")
        {
            std::string itemId, mealType, menuItem;
            std::vector<std::string> menuItemList;
            while(std::getline(ss, menuItem, ':'))
            {
                menuItemList.push_back(menuItem);
            }
            for(auto& item:menuItemList)
            {
                std::istringstream itemStream(item);
                std::string token, mealType;
                int itemId;
                std::getline(itemStream, token, '|');
                itemId = std::stoi(token);
                std::getline(itemStream, mealType, '|');

                std::cout<<"Adding daily menu: ";
                std::cout<<itemId<<" : "<<mealType<<std::endl;
                
                try
                {
                    std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement("INSERT INTO DailyMenu (item_id, meal_type) VALUES (?, ?)"));
                    pStatement->setInt(1, itemId);
                    pStatement->setString(2, mealType);
                    pStatement->execute();
                }
                catch (sql::SQLException &e)
                {
                    std::cerr << "MySQL error: "<< e.what() <<std::endl;
                }
            }
            response = "Daily Menu added successfully";
        }
        else if(command == "VIEW_ROLLED_OUT_MENU")
        {
            std::cout<<"Viewing rolled out menu"<<std::endl;
            auto rolledOutMenuItemList = getRolledOutMenuFromDatabase();

            std::string responseData="";
            for (auto& item : rolledOutMenuItemList)
            {
                responseData += item.getMenuDate() + ":" + std::to_string(item.getItemId()) + ":" + item.getItemName() + ":" + std::to_string(item.getItemPrice()) + ":" + std::to_string(item.getRating()) + "|";
            }

            if(!responseData.empty())
            {
                responseData.pop_back();
            }
            if(responseData.empty())
            {
                response = "Daily Menu not yet rolled out";
            }
            else
            {
                response = responseData;
            }
        }
        else if(command == "GET_RECOMMENDATION")
        {
            RecommendationEngine recommendation(database);
            auto recommendedItems = recommendation.getRecommendedMenuItems(2);

            std::string responseData;
            for (auto& item:recommendedItems)
            {
                responseData += std::to_string(item.first.getItemId()) + ":" + item.first.getItemName() + ":" + std::to_string(item.first.getItemPrice()) + ":" + std::to_string(item.first.getAvailabilityStatus()) + ":" + std::to_string(item.first.getRating()) + "|";
            }

            if (!responseData.empty())
            {
                responseData.pop_back();
            }

            response = responseData;
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
        int clientSocket = accpetingConnection();
        std::thread clientRequestThread(&SocketConnection::handleRequest, this, clientSocket);
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
            auto storedRole = result->getString("role");
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

bool SocketConnection::addMenuItemToDatabase(MenuItem &menuItem)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement("INSERT INTO MenuItems (item_id, item_name, price, availability_status) VALUES (?, ?, ?, ?)"));
        pStatement->setInt(1, menuItem.getItemId());
        pStatement->setString(2, menuItem.getItemName());
        pStatement->setInt(3, menuItem.getItemPrice());
        pStatement->setBoolean(4, menuItem.getAvailabilityStatus());
        pStatement->execute();
        return true;
    }
    catch(sql::SQLException& exception)
    {
        std::cerr<<"MySQL error: "<<exception.what()<<std::endl;
    }
    return false;
}

bool SocketConnection::deleteMenuItemFromDatabase(const int &itemId)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement("DELETE FROM MenuItems WHERE item_id = ?"));
        pStatement->setInt(1, itemId);
        pStatement->execute();
        return true;
    }
    catch(sql::SQLException& exception)
    {
        std::cerr<<"MySQL error: "<<exception.what()<<std::endl;
    }
    return false;
}

std::vector<MenuItem> SocketConnection::viewMenuItem()
{
    std::vector<MenuItem> menuItems;
    try
    {
        std::unique_ptr<sql::Statement> statement(database->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery("SELECT * FROM MenuItems"));
        while(result->next())
        {
            MenuItem item;
            item.setItemId(result->getInt("item_id"));
            item.setItemName(result->getString("item_name"));
            item.setItemPrice(result->getInt("price"));
            item.setAvailabilityStatus(result->getBoolean("availability_status"));
            menuItems.push_back(item);
        }
    }
    catch (sql::SQLException& exception)
    {
        std::cerr<<"MySQL error: "<<exception.what()<<std::endl;
    }
    return menuItems;
}

bool SocketConnection::addFeedbackToDatabase(Feedback &feedback)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pStatement(database->getConnection()->prepareStatement(
            "INSERT INTO Feedback (item_id, user_id, rating, comment) VALUES (?, ?, ?, ?)"));
        pStatement->setInt(1,feedback.getItemId());
        pStatement->setString(2,feedback.getUserId());
        pStatement->setInt(3,feedback.getRating());
        pStatement->setString(4,feedback.getComment());
        pStatement->execute();
        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "MySQL error: " << e.what() << std::endl;
    }
    return false;
}

std::vector<DailyMenu> SocketConnection::getRolledOutMenuFromDatabase()
{
    std::vector<DailyMenu> rolledOutMenu;

    try
    {
        std::unique_ptr<sql::Statement> statement(database->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery(
            "SELECT dm.menu_date, dm.item_id, mi.item_name, dm.meal_type, mi.price, "
            "IFNULL(AVG(f.rating), 0) AS average_rating "
            "FROM DailyMenu dm "
            "JOIN MenuItems mi ON dm.item_id = mi.item_id "
            "LEFT JOIN Feedback f ON dm.item_id = f.item_id "
            "WHERE dm.menu_date = CURRENT_DATE "
            "GROUP BY dm.menu_date, dm.item_id, dm.meal_type, mi.price"));

        while (result->next())
        {
            DailyMenu menu;
            menu.setMenuDate(result->getString("menu_date"));
            menu.setItemId(result->getInt("item_id"));
            menu.setItemName(result->getString("item_name"));
            menu.setMealType(result->getString("meal_type"));
            menu.setRating(result->getDouble("average_rating"));
            menu.setItemPrice(result->getInt("price"));
            rolledOutMenu.push_back(menu);
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "MySQL error: " << e.what() << std::endl;
    }
    return rolledOutMenu;
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
