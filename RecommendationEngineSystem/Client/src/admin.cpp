#include "admin.h"

Admin::Admin(const std::string &id, const std::string &name, const std::string &password)
    : adminId(id), adminName(name), password(password) {}

std::string Admin::requestToAddEmployees()
{
    std::string id, name, password, role;
    std::cout << "Enter the ID to be assigned:" << std::endl;
    std::cin >> id;
    std::cout << "Enter the name:" << std::endl;
    std::cin >> name;
    std::cout << "Enter the password:" << std::endl;
    std::cin >> password;
    std::cout<< "Enter the role:"<<std::endl;
    std::cin >> role;
    std::string message = "REGISTER_EMPLOYEE:" + id + ":" + name + ":" + password + ":" + role;
    return message;
}

std::string Admin::requestToDeleteEmployee()
{
    std::string userId;
    std::cout<<"Enter User ID to delete: ";
    std::cin>>userId;

    std::string message = "DELETE_USER:"+userId;
    return message;
}