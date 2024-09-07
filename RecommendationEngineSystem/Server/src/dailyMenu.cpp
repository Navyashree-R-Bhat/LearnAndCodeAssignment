#include "dailyMenu.h"

void DailyMenu::setItemId(const int& itemId)
{
    itemId_ = itemId;
}

void DailyMenu::setMenuDate(const std::string& menuDate)
{
    menuDate_ = menuDate;
}

void DailyMenu::setItemName(const std::string& itemName)
{
    itemName_ = itemName;
}

void DailyMenu::setMealType(const std::string& mealType)
{
    mealType_ = mealType;
}

void DailyMenu::setRating(const int& rating)
{
    rating_ = rating;
}

void DailyMenu::setFoodType(const std::string& foodType)
{
    foodType_ = foodType;
}

void DailyMenu::setSpiceLevel(const std::string& spiceLevel)
{
    spiceLevel_ = spiceLevel;
}

void DailyMenu::setCuisineType(const std::string& cuisineType)
{
    cuisineType_ = cuisineType;
}

void DailyMenu::setIsSweet(const bool& isSweet)
{
    isSweet_ = isSweet;
}

void DailyMenu::setItemPrice(const int& itemPrice)
{
    itemPrice_ = itemPrice;
}

int DailyMenu::getItemId()
{
    return itemId_;
}

std::string DailyMenu::getItemName()
{
    return itemName_;
}

std::string DailyMenu::getMealType()
{
    return mealType_;
}

std::string DailyMenu::getMenuDate()
{
    return menuDate_;
}

int DailyMenu::getRating()
{
    return rating_;
}

int DailyMenu::getItemPrice()
{
    return itemPrice_;
}

std::string DailyMenu::getFoodType()
{
    return foodType_;
}
std::string DailyMenu::getSpiceLevel()
{
    return spiceLevel_;
}
std::string DailyMenu::getCuisineType()
{
    return cuisineType_;
}
bool DailyMenu::getIsSweet()
{
    return isSweet_;
}
