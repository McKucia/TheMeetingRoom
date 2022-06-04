#pragma once

#include <string>
#include <vector>

#include "WorkingPeriod.hpp"

class Employee 
{
public:
    Employee(std::string _name) : name(_name) {}

    const std::string& getName() const { return name; }

private:
    std::string name;
    std::vector<WorkingPeriod> workingPeriods;
};