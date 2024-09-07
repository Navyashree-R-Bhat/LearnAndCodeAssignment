#include "chef.h"

Chef::Chef(const std::string &id, const std::string &name, const std::string &password)
    : chefId_(id), chefName_(name), password_(password) {}

std::string Chef::requestToViewMenuItem()
{
    std::string message = "VIEW_MENU_ITEMS";
    return message;
}

std::string Chef::requestToRollOutMenu()
{
    std::string message = "ROLL_OUT_MENU";
    return message;
}

std::string Chef::requestToViewVotes()
{
    std::string message = "VIEW_VOTES";
    return message;
}
