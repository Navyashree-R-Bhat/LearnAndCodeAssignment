#pragma once

#include <iostream>
#include <limits>

class Employee
{
private:
    std::string employeeId_;
    std::string employeeName_;
    std::string password_;
public:
    Employee(const std::string &id, const std::string &name, const std::string &password);
    std::string requestToViewMenuItem();
    std::string requestToProvideFeedback();
    std::string requestToViewRolledOutMenu();
    std::string requestToVoteForRolledOutMenu();
    std::string requestToUpdateEmployeeProfile();
    std::string requestToProvideFeedbackForDiscardedMenu();
    std::string requestToViewRolledOutMenuForParticularFoodType();
};
