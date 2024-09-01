#pragma once

#include <iostream>

class Employee
{
private:
    std::string employeeId;
    std::string employeeName;
    std::string password;
    std::string role;
public:
    Employee();
    Employee(const std::string& employeeId, const std::string& employeeName, const std::string& password, const std::string& role);
    std::string getEmployeeId();
    std::string getEmployeeName();
    std::string getPassword();
    std::string getRole();
};