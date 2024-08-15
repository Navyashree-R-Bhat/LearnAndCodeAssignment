#pragma once

#include<iostream>

class Chef
{
private:
    std::string chefId_;
    std::string chefName_;
    std::string password_;
public:
    Chef(const std::string &id, const std::string &name, const std::string &password);
    std::string requestToViewMenuItem();
    std::string requestToRollOutMenu();
};