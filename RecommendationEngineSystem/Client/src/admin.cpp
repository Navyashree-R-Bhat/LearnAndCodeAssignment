#include "admin.h"

Admin::Admin(const std::string &id, const std::string &name, const std::string &password)
    : adminId(id), adminName(name), password(password) {}

std::string Admin::requestToAddEmployees()
{
    std::string id, name, password, role;
    std::cout << "Enter the ID to be assigned:" << std::endl;
    std::cin >> id;
    std::cout << "Enter the name:" << std::endl;
    std::cin >> name;
    std::cout << "Enter the password:" << std::endl;
    std::cin >> password;
    std::cout<< "Enter the role:"<<std::endl;
    std::cin >> role;
    std::string message = "REGISTER_EMPLOYEE:" + id + ":" + name + ":" + password + ":" + role;
    return message;
}

std::string Admin::requestToDeleteEmployee()
{
    std::string userId;
    std::cout<<"Enter User ID to delete: ";
    std::cin>>userId;

    std::string message = "DELETE_USER:" + userId;
    return message;
}

std::string Admin::requestToAddMenuItem()
{
    int itemId, itemPrice;
    std::string itemName, foodType, spiceLevel, cuisineType;
    char availabilityStatusUserInput, sweetToothUserInput;
    bool availabilityStatus, isSweetTooth;

    while(true)
    {
        std::cout<<"Enter the ID to be assigned to the item: ";
        std::cin>>itemId;
        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        } 
        std::cout<<"Enter the name of the item: ";
        std::cin>>itemName;
        std::cout<<"Enter the price for the item: ";
        std::cin>>itemPrice;
        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        } 
        std::cout<<"Enter the availability status of the item[y/n]: ";
        std::cin>>availabilityStatusUserInput;
        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid option." << std::endl;
            continue;
        }
        availabilityStatusUserInput = std::tolower(availabilityStatusUserInput);

        if (availabilityStatusUserInput == 'y')
        {
            availabilityStatus = true;
        }
        else if (availabilityStatusUserInput == 'n')
        {
            availabilityStatus = false;
        }
        else
        {
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << std::endl;
            continue;
        }

        std::cout<<"Enter the Food Type['Vegetarian', 'Non Vegetarian', 'Eggetarian']: ";
        std::getline(std::cin, foodType);
        if(foodType != "Vegetarian" && foodType != "Non Vegetarian" && foodType != "Eggetarian")
        {
            std::cout<<"Invalid option entered"<<std::endl;
            continue;
        }
        std::cout<<"Enter the Spice Level['High', 'Medium', 'Low']: ";
        std::getline(std::cin, spiceLevel);
        if(spiceLevel != "High" && spiceLevel != "Medium" && spiceLevel != "Low")
        {
            std::cout<<"Invalid option entered"<<std::endl;
            continue;
        }
        std::cout<<"Enter the Cuisine Type['North Indian', 'South Indian', 'Other']: ";
        std::getline(std::cin, cuisineType);
        if(cuisineType != "North Indian" && cuisineType != "South Indian" && cuisineType != "Other")
        {
            std::cout<<"Invalid option entered"<<std::endl;
            continue;
        }
        std::cout<<"Enter the sweet tooth[y/n]: ";
        std::cin>>sweetToothUserInput;
        sweetToothUserInput = std::tolower(sweetToothUserInput);
        if (std::cin.fail() && sweetToothUserInput != 'y' && sweetToothUserInput != 'n') 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid option." << std::endl;
            continue;
        }

        if (sweetToothUserInput == 'y')
        {
            isSweetTooth = true;
        }
        else if (sweetToothUserInput == 'n')
        {
            isSweetTooth = false;
        }
        else
        {
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << std::endl;
            continue;
        }
        break;
    }

    std::string message = "ADD_MENU_ITEM:" + std::to_string(itemId) + ":" + itemName + ":" + std::to_string(itemPrice) + ":" + (availabilityStatus ? "true":"false");
    return message;
}

std::string Admin::requestToDeleteMenuItem()
{
    int itemId;
    std::cout<<"Enter menu item ID to delete: ";
    std::cin>>itemId;
    
    std::string message = "DELETE_MENU_ITEM:"+std::to_string(itemId);
    return message;
}

std::string Admin::requestToViewMenuItem()
{
    std::string message = "VIEW_MENU_ITEMS";
    return message;
}