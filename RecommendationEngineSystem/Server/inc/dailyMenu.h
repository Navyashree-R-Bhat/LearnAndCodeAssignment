#pragma once

#include <iostream>

class DailyMenu
{
private:
    int itemId_, rating_, itemPrice_;
    std::string menuDate_, itemName_, mealType_;
public:
    void setItemId(const int& itemId);
    void setMenuDate(const std::string& menuDate);
    void setItemName(const std::string& itemName);
    void setMealType(const std::string& mealType);
    void setRating(const int& rating);
    void setItemPrice(const int& itemPrice);

    int getItemId();
    std::string getMenuDate();
    std::string getItemName();
    std::string getMealType();
    int getRating();
    int getItemPrice();
};