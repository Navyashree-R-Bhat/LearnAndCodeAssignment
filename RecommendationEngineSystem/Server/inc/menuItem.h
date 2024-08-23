#pragma once

#include <iostream>

class MenuItem
{
private:
    int itemId_;
    std::string itemName_, comment_;
    int itemPrice_;
    bool availabilityStatus_;
    int rating_;
public:
    MenuItem();
    MenuItem(const int& itemId, const std::string& itemName, const int& itemPrice, const bool& availabilityStatus);
    void setItemId(const int& itemId);
    void setItemName(const std::string& itemName);
    void setItemPrice(const int& itemPrice);
    void setAvailabilityStatus(const bool& availabilityStatus);
    void setRating(const int& rating);
    int getItemId();
    std::string getItemName();
    int getItemPrice();
    bool getAvailabilityStatus();
    int getRating();
};