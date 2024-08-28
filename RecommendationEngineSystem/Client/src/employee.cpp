#include "employee.h"

Employee::Employee(const std::string &id, const std::string &name, const std::string &password)
    : employeeId_(id), employeeName_(name), password_(password) {}

std::string Employee::requestToViewMenuItem()
{
    std::string message = "VIEW_MENU_ITEMS";
    return message;
}

std::string Employee::requestToProvideFeedback()
{
    int itemId, rating;
    std::string comment;

    std::cout<<"Enter Item ID: ";
    std::cin>>itemId;
    std::cout<<"Enter rating(0-5): ";
    std::cin>>rating;
    std::cin.ignore();
    std::cout<<"Enter comment: ";
    std::getline(std::cin, comment);

    std::string message = "ADD_FEEDBACK:" + std::to_string(itemId) + ":" + employeeId_ + ":" + std::to_string(rating) + ":" + comment;

    return message;
}

std::string Employee::requestToViewRolledOutMenu()
{
    std::string message = "VIEW_ROLLED_OUT_MENU";
    return message;
}

std::string Employee::requestToVoteForRolledOutMenu()
{
    int itemId;
    std::string message;
    while(true)
    {
        std::cout<<"Enter the item id you wish to vote for: ";
        std::cin>>itemId;
        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        }
        else
        {
            break;
        }
    }
    message = "VOTE:" + std::to_string(itemId) + ":" + employeeId_;
    return message;
}