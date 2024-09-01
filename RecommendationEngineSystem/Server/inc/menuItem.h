#pragma once

#include <iostream>

class MenuItem
{
private:
    int itemId_;
    std::string itemName_, comment_, foodType_, spiceLevel_, cuisineType_;
    int itemPrice_;
    bool availabilityStatus_, isSweet_;
    int rating_;
public:
    MenuItem();
    MenuItem(const int& itemId, const std::string& itemName, const int& itemPrice, const bool& availabilityStatus);
    void setItemId(const int& itemId);
    void setItemName(const std::string& itemName);
    void setItemPrice(const int& itemPrice);
    void setAvailabilityStatus(const bool& availabilityStatus);
    void setRating(const int& rating);
    void setFoodType(const std::string& foodType);
    void setSpiceLevel(const std::string& spiceLevel);
    void setCuisineType(const std::string& cuisineType);
    void setIsSweet(const bool& isSweet);
    int getItemId();
    std::string getItemName();
    int getItemPrice();
    bool getAvailabilityStatus();
    int getRating();
    std::string getFoodType();
    std::string getSpiceLevel();
    std::string getCuisineType();
    bool getIsSweet();
};