#include "employee.h"

Employee::Employee(const std::string &id, const std::string &name, const std::string &password)
    : employeeId_(id), employeeName_(name), password_(password) {}

std::string Employee::requestToViewMenuItem()
{
    std::string message = "VIEW_MENU_ITEMS";
    return message;
}

std::string Employee::requestToProvideFeedback()
{
    int itemId, rating;
    std::string comment;

    std::cout<<"Enter Item ID: ";
    std::cin>>itemId;
    std::cout<<"Enter rating(0-5): ";
    std::cin>>rating;
    std::cin.ignore();
    std::cout<<"Enter comment: ";
    std::getline(std::cin, comment);

    std::string message = "ADD_FEEDBACK:" + std::to_string(itemId) + ":" + employeeId_ + ":" + std::to_string(rating) + ":" + comment;

    return message;
}

std::string Employee::requestToViewRolledOutMenu()
{
    std::string message = "VIEW_ROLLED_OUT_MENU:" + employeeId_;
    return message;
}

std::string Employee::requestToVoteForRolledOutMenu()
{
    int itemId;
    std::string message;
    while(true)
    {
        std::cout<<"Enter the item id you wish to vote for: ";
        std::cin>>itemId;
        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        }
        else
        {
            break;
        }
    }
    message = "VOTE:" + std::to_string(itemId) + ":" + employeeId_;
    return message;
}

std::string Employee::requestToUpdateEmployeeProfile()
{
    std::string message = "UPDATE_PROFILE:";
    std::string foodPreference, spiceLevel, cuisinePreference;
    char sweetToothUserInput;
    bool isSweetTooth;
    std::cin.ignore();
    while(true)
    {
        std::cout<<"Enter the Food Preference['Vegetarian', 'Non Vegetarian', 'Eggetarian']: ";
        std::getline(std::cin, foodPreference);
        if(foodPreference != "Vegetarian" && foodPreference != "Non Vegetarian" && foodPreference != "Eggetarian")
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
        std::cout<<"Enter the Cuisine Preference['North Indian', 'South Indian', 'Other']: ";
        std::getline(std::cin, cuisinePreference);
        if(cuisinePreference != "North Indian" && cuisinePreference != "South Indian" && cuisinePreference != "Other")
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
    message += employeeId_ + ":" + foodPreference + ":" + spiceLevel + ":" + cuisinePreference + ":" + std::to_string(isSweetTooth);
    return message;
}

std::string Employee::requestToProvideFeedbackForDiscardedMenu()
{
    int itemId;
    std::string answer1, answer2, answer3;

    std::cout<<"Enter Item ID: ";
    std::cin>>itemId;
    std::cin.ignore();
    std::cout<<"What didn’t you like about Food Item: ";
    std::getline(std::cin, answer1);
    std::cout<<"How would you like Food Item to taste: ";
    std::getline(std::cin, answer2);
    std::cout<<"Share your mom’s recipe: ";
    std::getline(std::cin, answer3);
    std::string message = "ADD_DISCARD_FEEDBACK:" + std::to_string(itemId) + ":" + employeeId_ + ":" + answer1 + ":" + answer2 + ":" + answer3;

    return message;

}

std::string Employee::requestToViewRolledOutMenuForParticularFoodType()
{
    std::string message = "VIEW_ROLLED_OUT_MENU_FOR_FOOD_TYPE:" + employeeId_;
    return message;
}
