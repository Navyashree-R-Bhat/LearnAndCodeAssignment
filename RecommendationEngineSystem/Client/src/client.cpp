#include "client.h"

void Client::start()
{
    while(1)
    {
        int choice;
        std::cout<<"************CAFETERIA************"<<std::endl;
        std::cout<<"Users: "<<std::endl;
        std::cout<<"1.ADMIN\n2.CHEF\n3.EMPLOYEE"<<std::endl;
        std::cout<<"Enter the choice to login: ";
        std::cin>>choice;

        switch(choice)
        {
            case 1:
                //admin
                break;
            case 2:
                //Chef
                break;
            case 3:
                //Employee
                break;
            default:
                std::cout<<"You entered an invalid choice."<<std::endl;
        }
    }
}

void Client::adminDisplayScreen()
{
    //admin
    std::string userName, password;
    std::cout<<"Enter user name: ";
    std::cin>>userName;
    std::cout<<"\nEnter password: ";
    std::cin>>password;
    //authenticate
}