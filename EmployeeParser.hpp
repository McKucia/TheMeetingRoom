#pragma once

#include <string>
#include <vector>

#include "Structures.hpp"
#include "Employee.hpp"

class EmployeeParser
{
public:
    std::vector<Employee> parseEmployees(table employeesTable)
    {
        for(const std::vector<std::string>& row : employeesTable)
        {
            if ( std::any_of(employees.begin(), employees.end(), [ row ](Employee& e){
                return e.getName() == row[0];
            }) )
            {
                auto it = std::find_if(employees.begin(), employees.end(),  [ row ](Employee& e) { 
                    return e.getName() == row[0]; 
                });

                Employee& employee = *it;

                WorkingPeriod workingPeriod = parseHours(row);
                employee.addWorkingPeriod(workingPeriod);
            }
            else
            {
                Employee employee { row[0] }; // name
                
                WorkingPeriod workingPeriod = parseHours(row);
                employee.addWorkingPeriod(workingPeriod);

                employees.push_back(employee);
            }

        }

        return employees;
    }

private:
    std::vector<Employee> employees;

    WorkingPeriod parseHours(const std::vector<std::string>& row)
    {
        std::string start = row[1], finish = row[2];
        
        float startMinutes = std::stof(start.substr(start.length() - 2)) / 60;
        float startHour = std::stof(start.substr(0, start.find(":")));
        float startParsed = startHour + startMinutes;

        float finishMinutes = std::stof(finish.substr(finish.length() - 2)) / 60;
        float finishHour = std::stof(finish.substr(0, finish.find(":")));
        float finishParsed = finishHour + finishMinutes;

        WorkingPeriod workingPeriod{ startParsed, finishParsed };
        return workingPeriod;
    }
};