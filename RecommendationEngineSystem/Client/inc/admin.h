#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <limits>

class Admin
{
private:
    std::string adminId;
    std::string adminName;
    std::string password;

public:
    Admin(const std::string &id, const std::string &name, const std::string &password);

    std::string requestToAddEmployees();
    std::string requestToDeleteEmployee();
    std::string requestToAddMenuItem();
    std::string requestToDeleteMenuItem();
    std::string requestToViewMenuItem();
};