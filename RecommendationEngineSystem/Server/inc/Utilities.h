#pragma once

#include <iostream>

class Utilities
{
public:
    static std::string getRoleEnum(const std::string &role);
    static bool stringToBool(const std::string& str);
};