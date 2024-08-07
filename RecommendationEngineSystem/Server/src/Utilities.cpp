#include "Utilities.h"

int Utilities::getRoleEnum(const std::string &role)
{
    if(role=="admin")
    {
        return 0;
    }
    else if(role=="chef")
    {
        return 1;
    }
    else if(role=="employee")
    {
        return 2;
    }
    else
    {
        throw std::runtime_error("Unknown role integer: ");
    }
}