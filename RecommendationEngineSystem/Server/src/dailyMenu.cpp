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