#include <string>
#include <vector>

#include "WorkingPeriod.hpp"
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
        start.replace(start.find(":"), 1, ".");
        finish.replace(finish.find(":"), 1, ".");

        WorkingPeriod workingPeriod{ std::stof(start), std::stof(finish) };
        return workingPeriod;
    }
};