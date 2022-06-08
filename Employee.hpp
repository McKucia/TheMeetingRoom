#pragma once

#include <string>
#include <vector>

#include "Structures.hpp"

class Employee 
{
public:
    Employee(std::string _name) : name(_name) {}
    Employee() {}

    const std::vector<WorkingPeriod>& getWorkingPeriods() const { return workingPeriods; }
    const std::string& getName()                          const { return name; }

    void addWorkingPeriod(const WorkingPeriod& workingPeriod)
    {
        workingPeriods.push_back(workingPeriod);
    }

private:
    std::string name;
    std::vector<WorkingPeriod> workingPeriods;
};