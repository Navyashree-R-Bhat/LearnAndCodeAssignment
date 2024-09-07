#include "menuItem.h"

MenuItem::MenuItem(){}

MenuItem::MenuItem(const int& itemId, const std::string& itemName, const int& itemPrice, const bool& availabilityStatus):
    itemId_(itemId), itemName_(itemName), itemPrice_(itemPrice), availabilityStatus_(availabilityStatus) { }

void MenuItem::setItemId(const int& itemId)
{
    itemId_ = itemId;
}

void MenuItem::setItemName(const std::string& itemName)
{
    itemName_ = itemName;
}

void MenuItem::setItemPrice(const int& itemPrice)
{
    itemPrice_ = itemPrice;
}

void MenuItem::setAvailabilityStatus(const bool& availabilityStatus)
{
    availabilityStatus_ = availabilityStatus;
}

void MenuItem::setRating(const int& rating)
{
    rating_ = rating;
}

void MenuItem::setFoodType(const std::string& foodType)
{
    foodType_ = foodType;
}

void MenuItem::setSpiceLevel(const std::string& spiceLevel)
{
    spiceLevel_ = spiceLevel;
}

void MenuItem::setCuisineType(const std::string& cuisineType)
{
    cuisineType_ = cuisineType;
}

void MenuItem::setIsSweet(const bool& isSweet)
{
    isSweet_ = isSweet;
}

int MenuItem::getItemId()
{
    return itemId_;
}

std::string MenuItem::getItemName()
{
    return itemName_;
}

int MenuItem::getItemPrice()
{
    return itemPrice_;
}

bool MenuItem::getAvailabilityStatus()
{
    return availabilityStatus_;
}

int MenuItem::getRating()
{
    return rating_;
}

std::string MenuItem::getFoodType()
{
    return foodType_;
}
std::string MenuItem::getSpiceLevel()
{
    return spiceLevel_;
}
std::string MenuItem::getCuisineType()
{
    return cuisineType_;
}
bool MenuItem::getIsSweet()
{
    return isSweet_;
}
