#include "client.h"

void Client::sendRequestToServer(const std::string& request)
{
    send(client.clientSocket, request.c_str(), request.size(), 0);

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
    std::cout<<response<<std::endl;
}

void Client::start()
{
    while (1)
    {
        int choice;
        std::cout << "************CAFETERIA************" << std::endl;
        std::cout << "Users: " << std::endl;
        std::cout << "1.ADMIN\n2.CHEF\n3.EMPLOYEE\n4.Exit Application" << std::endl;
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
                adminDisplayScreen();
                break;
            case 2:
                chefDisplayScreen();
                break;
            case 3:
                employeeDisplayScreen();
                break;
            case 4:
                return;
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
    send(client.clientSocket, request.c_str(), request.length(), 0);
    const int bufferSize = 1024;
    char buffer[bufferSize] = {0};
    read(client.clientSocket, buffer, sizeof(buffer));

    if (!strcmp(buffer, "success"))
    {
        status = true;
    }
    return status;
}

void Client::adminDisplayScreen()
{
    std::string userId, password;
    std::cout << "Enter user id: ";
    std::cin >> userId;
    std::cout << "Enter password: ";
    std::cin >> password;

    if(authenticateUser(userId, password, "admin"))
    {
        Admin admin(userId, "ADMIN", password);
        int choice = 0;
        while(choice != 7)
        {
            std::cout<<"\nWelcome to Admin page"<<std::endl;
            std::cout<<"Operations"<<std::endl;
            std::cout<<"1.Add User"<<std::endl;
            std::cout<<"2.Add Food Item"<<std::endl;
            std::cout<<"3.View Menu"<<std::endl;
            std::cout<<"4.Delete User"<<std::endl;
            std::cout<<"5.Delete Food Item"<<std::endl;
            std::cout<<"6.Discard Menu Item List"<<std::endl;
            std::cout<<"7.Log out"<<std::endl;
            std::cout<<"Enter your choice: ";
            std::cin>>choice;
            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                        request = admin.requestToAddMenuItem();
                        sendRequestToServer(request);
                        sendRequestToAddNotification("New Food item added");
                        break;
                    case 3:
                        request = admin.requestToViewMenuItem();
                        sendRequestToServerToViewMenuItem(request);
                        break;
                    case 4:
                        request = admin.requestToDeleteEmployee();
                        sendRequestToServer(request);
                        break;
                    case 5:
                        request = admin.requestToDeleteMenuItem();
                        sendRequestToServer(request);
                        break;
                    case 6:
                        sendRequestToDiscardMenuItem();
                        performActionForDiscardMenu();
                        break;
                    case 7:
                        break;
                    default:
                        std::cout << "You entered an invalid choice." << std::endl;
                }
            }
        }
    }
    else
    {
        std::cout<<"Invalid credentials\n"<<std::endl;
    }
}

void Client::sendRequestToServerToViewMenuItem(const std::string& request)
{
    send(client.clientSocket, request.c_str(), request.size(), 0);
    std::cout<<"View menu request sent"<<std::endl;

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);

    std::string menuItemResponse(response);

    std::istringstream menuItemStream(menuItemResponse);
    std::string menuItem;
    std::vector<std::string> menuItemList;

    while(std::getline(menuItemStream, menuItem, '|'))
    {
        menuItemList.push_back(menuItem);
    }

    std::cout<<"MENU LIST"<<std::endl;
    for (auto& item : menuItemList)
    {
        std::istringstream itemStream(item);
        std::string token;
        std::vector<std::string> fields;
        while(std::getline(itemStream, token, ':'))
        {
            fields.push_back(token);
        }
        if (fields.size() == 4)
        {
            std::cout << "ID: " << fields[0]
                      << " Name: " << fields[1]
                      << "\tPrice: " << fields[2]
                      << " Availability: " << (fields[3] == "1" ? "Yes" : "No") << std::endl;
        }   }
}

void Client::sendRequestToAddNotification(const std::string &notificationMessage)
{   
    std::string request;
    request = "ADD_NOTIFICATION:" + notificationMessage;
    sendRequestToServer(request);
}

void Client::sendRequestToViewNotification(const std::string &userId)
{
    std::string request = "GET_NOTIFICATIONS:" + userId;
    send(client.clientSocket, request.c_str(), request.size(), 0);

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);

    std::istringstream notificationStream(response);
    std::string notification, responseMessage;
    std::vector<std::string> notificationsList;

    std::cout<<"\nNotifications\n";
    std::getline(notificationStream, responseMessage, ':');
    std::cout<<responseMessage<<std::endl;
    while(std::getline(notificationStream, notification, '|'))
    {
        notificationsList.push_back(notification);
    }
    for(const auto &notificationMessage : notificationsList)
    {
        std::istringstream notificationMessageStream(notificationMessage);
        std::string notification;
        while(std::getline(notificationMessageStream, notification, ':'))
        {
            std::cout<<"--->"<<notification<<std::endl;
        }
    }
    sendRequestToDeleteNotifications(userId);
}

void Client::sendRequestToDeleteNotifications(const std::string &userId)
{
    std::string request = "DELETE_NOTIFICATIONS:" + userId;
    send(client.clientSocket, request.c_str(), request.size(), 0);

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
}

void Client::chefDisplayScreen()
{
    std::string userId, password;
    std::cout<<"Enter user id: ";
    std::cin>>userId;
    std::cout<<"Enter password: ";
    std::cin>>password;
    if (authenticateUser(userId, password, "chef"))
    {
        Chef chef(userId, "CHEF", password);
        int choice = 0;
        while(choice != 5)
        {
            std::cout<<"\nWelcome to Chef page"<<std::endl;
            std::cout<<"Operations"<<std::endl;
            std::cout<<"1.View Menu"<<std::endl;
            std::cout<<"2.Get Recommendation"<<std::endl;
            std::cout<<"3.Roll out Menu Items"<<std::endl;
            std::cout<<"4.View Votes"<<std::endl;
            std::cout<<"5.Log out"<<std::endl;
            std::cout<<"Enter your choice: ";
            std::cin>>choice;
            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
                continue;
            }
            else
            {
                std::string request;
                switch(choice)
                {
                    case 1:
                        request = chef.requestToViewMenuItem();
                        sendRequestToServerToViewMenuItem(request);
                        break;
                    case 2:
                        getRecommendations();
                        break;
                    case 3:
                        request = chef.requestToRollOutMenu();
                        sendRequestToServerToRollOutMenu(request);
                        sendRequestToAddNotification("Daily Menu Rolled Out");
                        break;
                    case 4:
                        request = chef.requestToViewVotes();
                        sendRequestToViewVotes(request);
                        break;
                    case 5:
                        break;
                    default:
                        std::cout << "You entered an invalid choice." << std::endl;
                }
            }
        }
    }
}

void Client::employeeDisplayScreen()
{
    std::string userId, userName, password;
    std::cout<<"Enter user id: ";
    std::cin>>userId;
    std::cout<<"Enter password: ";
    std::cin>>password;
    if (authenticateUser(userId, password, "employee"))
    {
        sendRequestToViewNotification(userId);
        Employee employee(userId, userName, password);
        std::cout<<"\nWelcome to Employee page"<<std::endl;
        int choice = 0;
        while(choice != 7)
        {
            std::cout<<"Operations"<<std::endl;
            std::cout<<"1.View Menu"<<std::endl;
            std::cout<<"2.View Daily Menu"<<std::endl;
            std::cout<<"3.Provide feedback"<<std::endl;
            std::cout<<"4.Vote for Rolled out MenuItem"<<std::endl;
            std::cout<<"5.Update Profile"<<std::endl;
            std::cout<<"6.Provide feedback for discard menu item"<<std::endl;
            std::cout<<"7.Log out"<<std::endl;
            std::cout<<"Enter your choice: ";
            std::cin>>choice;
            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
                continue;
            }
            else
            {
                std::string request;
                switch(choice)
                {
                    case 1:
                        request = employee.requestToViewMenuItem();
                        sendRequestToServerToViewMenuItem(request);
                        break;
                    case 2:
                        request = employee.requestToViewRolledOutMenuForParticularFoodType();
                        sendRequestToServerToViewRolledOutMenu(request);
                        requestForViewingEntireMenu(employee);
                        break;
                    case 3:
                        request = employee.requestToProvideFeedback();
                        sendRequestToServer(request);
                        break;
                    case 4:
                        request = employee.requestToVoteForRolledOutMenu();
                        sendRequestToServer(request);
                        break;
                    case 5:
                        request = employee.requestToUpdateEmployeeProfile();
                        sendRequestToServer(request);
                        break;
                    case 6:
                        sendRequestToDiscardMenuItem();
                        request = employee.requestToProvideFeedbackForDiscardedMenu();
                        sendRequestToServer(request);
                        break;
                    case 7:
                        break;
                    default:
                        std::cout << "You entered an invalid choice." << std::endl;
                }
            }
        }
    }
}

void Client::sendFeedback()
{
    int item_id, rating;
    std::string user_id, comment;

    std::cout<<"Enter Item ID: ";
    std::cin>>item_id;
    std::cout<<"Enter user_id: ";
    std::cin>>user_id;
    std::cout<<"Enter rating(0-5): ";
    std::cin>>rating;
    std::cin.ignore();
    std::cout<<"Enter comment: ";
    std::getline(std::cin, comment);

    std::string message = "ADD_FEEDBACK:" + std::to_string(item_id) + ":" + user_id + ":" + std::to_string(rating) + ":" + comment;
    send(client.clientSocket, message.c_str(), message.length(), 0);
    std::cout<<"Feedback sent"<<std::endl;

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
    std::cout<<"Response from server: "<<response<<std::endl;
}

void Client::sendRequestToServerToRollOutMenu(const std::string& message)
{
    int itemId;
    std::string mealType;
    std::string request = message + ':';
    
    while(true)
    {
        std::cout<<"Enter Item ID: ";
        std::cin>>itemId;
        std::cout<<"Enter meal type(Breakfast/Lunch/Dinner): ";
        std::cin>>mealType;
        if (mealType != "Breakfast" && mealType != "Lunch" && mealType != "Dinner")
        {
            std::cout<<"Invalid meal type. Please enter 'Breakfast', 'Lunch', or 'Dinner'" << std::endl;
            continue;
        }
        request += std::to_string(itemId) + '|' + mealType + ':';
        char option;
        std::cout<<"Do you want to continue y/n: ";
        option = std::tolower(option);
        if (std::cin.fail() && option != 'y' && option != 'n') 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid option." << std::endl;
            continue;
        }

        if (option == 'y')
        {
            continue;
        }
        else if (option == 'n')
        {
            break;
        }
        else
        {
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << std::endl;
            continue;
        }
        std::cin.ignore();
    }

    send(client.clientSocket, request.c_str(), request.length(), 0);
    std::cout<<"Roll Out Menu Request sent"<<std::endl;

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
    std::cout<<"Response from server: "<<response<<std::endl;
}

void Client::sendRequestToServerToViewRolledOutMenu(const std::string& message)
{
    send(client.clientSocket, message.c_str(), message.length(), 0);

    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
    
    std::istringstream menuStream(response);
    std::string menuItem, responseMessage;
    std::vector<std::string> menuItems;

    std::cout<<"\nDAILY MENU LIST\n";
    std::getline(menuStream, responseMessage, ':');
    std::cout<<responseMessage<<std::endl;
    while(std::getline(menuStream, menuItem, '|'))
    {
        menuItems.push_back(menuItem);
    }
    for(const auto &item : menuItems)
    {
        std::istringstream itemStream(item);
        std::string token;
        std::vector<std::string> fields;
        while(std::getline(itemStream, token, ':'))
        {
            fields.push_back(token);
        }
        if(fields.size() == 10)
        {
            std::cout <<"Date: " << fields[0]
                    << " ID: " << fields[1]
                    << " Name: " << fields[2]
                    << " Price: " <<fields[3]
                    << " Rating: " << fields[4] 
                    << " Meal type: " << fields[5]
                    << " Food type: " <<fields[6]
                    << " Spice level: "<<fields[7]
                    << " Cuisine type: " <<fields[8]
                    << " Is sweet: " << (fields[9] == "1" ? "Yes" : "No") << std::endl;
        }
    }

}

void Client::getRecommendations()
{
    std::string request = "GET_RECOMMENDATION:";
    int noOfRecommendations;
    std::cout<<"Enter number of items to be recommended: ";
    std::cin>>noOfRecommendations;
    request += std::to_string(noOfRecommendations);
    send(client.clientSocket, request.c_str(), request.length(), 0);
    std::cout << "Recommendation request sent\n";

    int bufferSize = 1024;
    char buffer[bufferSize] = {0};
    read(client.clientSocket, buffer, bufferSize);

    std::string response(buffer);

    std::istringstream recommendationsStream(response);
    std::string recommendation;
    std::vector<std::string> recommendations;
    while (std::getline(recommendationsStream, recommendation, '|'))
    {
        recommendations.push_back(recommendation);
    }

    std::cout << "Recommendations received:\n";
    for (const auto &rec : recommendations)
    {
        std::istringstream recommendationStream(rec);
        std::string token;
        std::vector<std::string> fields;
        while (std::getline(recommendationStream, token, ':'))
        {
            fields.push_back(token);
        }

        if (fields.size() == 5)
        {
            std::cout << "Item ID: " << fields[0] << "\n";
            std::cout << "Food Item: " << fields[1] << "\n";
            std::cout << "Price: " << fields[2] << "\n";
            std::cout << "Rating: " << fields[4] << "\n";
            std::cout << "Availability: " << (fields[3] == "1" ? "Yes" : "No") << "\n";
            std::cout << "------------------\n";
        }
    }
}

void Client::sendRequestToViewVotes(const std::string &request)
{
    send(client.clientSocket, request.c_str(), request.length(), 0);
    
    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);

    std::istringstream voteListStream(response);
    std::string itemVote;
    std::vector<std::string> voteList;

    std::cout<<"\nVOTING LIST\n";
    while(std::getline(voteListStream, itemVote, '|'))
    {
        voteList.push_back(itemVote);
    }
    for(const auto &votedItem : voteList)
    {
        std::istringstream voteStream(votedItem);
        std::string token;
        std::vector<std::string> fields;
        while(std::getline(voteStream, token, ':'))
        {
            fields.push_back(token);
        }
        if (fields.size() == 3)
        {
            std::cout<<"ID: "<<fields[0]
                     <<" Name: "<<fields[1]
                     <<" No of Votes: "<<fields[2] <<std::endl;
        }  
    }
}

void Client::sendRequestToDiscardMenuItem()
{
    std::string request = "VIEW_DISCARD_MENU";
    send(client.clientSocket, request.c_str(), request.length(), 0);
    int bufferSize = 1024;
    char response[bufferSize] = {0};
    read(client.clientSocket, response, bufferSize);
    std::istringstream discardedMenuItemStream(response);
    std::string menuItem;
    std::vector<std::string> discardMenu;
    while(std::getline(discardedMenuItemStream, menuItem, '|'))
    {
        discardMenu.push_back(menuItem);
    }
    std::cout<<"DISCARD MENU LIST"<<std::endl;
    for(const auto &item : discardMenu)
    {
        std::istringstream itemStream(item);
        std::string token;
        std::vector<std::string> fields;
        while(std::getline(itemStream, token, ':'))
        {
            fields.push_back(token);
        }

        if (fields.size() == 5)
        {
            std::cout << "Item ID: " << fields[0] << "\n";
            std::cout << "Food Item: " << fields[1] << "\n";
            std::cout << "Price: " << fields[2] << "\n";
            std::cout << "Rating: " << fields[4] << "\n";
            std::cout << "Availability: " << (fields[3] == "1" ? "Yes" : "No") << "\n";
            std::cout << "------------------\n";
        }
    }
}

void Client::sendRequestToDeleteDiscardMenu()
{
    int itemId;
    std::cout<<"Enter the item ID to be deleted: ";
    std::cin>>itemId;
    std::string request = "DELETE_DISCARDED_MENU_ITEM:" + std::to_string(itemId);
    sendRequestToServer(request);
}

void Client::performActionForDiscardMenu()
{
    int choice;
    std::cout<<"How do you want to continue"<<std::endl;
    std::cout<<"1.Discard Menu Item"<<std::endl;
    std::cout<<"2.Receive Feedback from employee"<<std::endl;
    std::cin>>choice;
    if (choice == 1)
    {
        sendRequestToDeleteDiscardMenu();
    }
    else if (choice == 2)
    {
        sendRequestToAddNotification("Give suggestion for food item");
    }
}

void Client::requestForViewingEntireMenu(Employee& employee)
{
    char option;
    while(true)
    {
        std::cout<<"Do you want to view entire menu [y/n]: ";
        std::cin>>option;
        option = std::tolower(option);
        if (std::cin.fail() && option != 'y' && option != 'n') 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid option." << std::endl;
            continue;
        }

        if (option == 'y')
        {
            auto request = employee.requestToViewRolledOutMenu();
            sendRequestToServerToViewRolledOutMenu(request);
            break;
        }
        else if (option == 'n')
        {
            break;
        }
        else
        {
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << std::endl;
            continue;
        }
    }
}
