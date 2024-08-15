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

    return message;
}

std::string Employee::requestToViewRolledOutMenu()
{
    std::string message = "VIEW_ROLLED_OUT_MENU";
    return message;
}