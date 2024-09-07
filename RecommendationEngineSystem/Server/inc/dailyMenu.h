#pragma once

#include <iostream>

class DailyMenu
{
private:
    int itemId_, rating_, itemPrice_;
    std::string menuDate_, itemName_, mealType_, foodType_, spiceLevel_, cuisineType_;
    bool isSweet_;
public:
    void setItemId(const int& itemId);
    void setMenuDate(const std::string& menuDate);
    void setItemName(const std::string& itemName);
    void setMealType(const std::string& mealType);
    void setRating(const int& rating);
    void setItemPrice(const int& itemPrice);
    void setFoodType(const std::string& foodType);
    void setSpiceLevel(const std::string& spiceLevel);
    void setCuisineType(const std::string& cuisineType);
    void setIsSweet(const bool& isSweet);

    int getItemId();
    std::string getMenuDate();
    std::string getItemName();
    std::string getMealType();
    int getRating();
    int getItemPrice();
    std::string getFoodType();
    std::string getSpiceLevel();
    std::string getCuisineType();
    bool getIsSweet();
};
