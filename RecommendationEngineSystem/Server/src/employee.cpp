#include "employee.h"

Employee::Employee()
{
}

Employee::Employee(const std::string& employeeId, const std::string& employeeName, const std::string& password, const std::string& role)
                   : employeeId(employeeId), employeeName(employeeName), password(password), role(role) {}

std::string Employee::getEmployeeId()
{
    return employeeId;
}

std::string Employee::getEmployeeName()
{
    return employeeName;
}

std::string Employee::getPassword()
{
    return password;
}

std::string Employee::getRole()
{
    return role;
}