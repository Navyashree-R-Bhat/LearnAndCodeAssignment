#include "Utilities.h"

std::string Utilities::getRoleEnum(const std::string &role)
{
    if(role=="admin")
    {
        return "admin";
    }
    else if(role=="chef")
    {
        return "chef";
    }
    else if(role=="employee")
    {
        return "employee";
    }
    else
    {
        throw std::runtime_error("Unknown role integer: ");
    }
}

bool Utilities::stringToBool(const std::string& str) {
    return str == "true" || str == "1";
}