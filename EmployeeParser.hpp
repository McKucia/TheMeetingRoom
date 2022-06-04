#include <string>
#include <vector>

#include "WorkingPeriod.hpp"
#include "Employee.hpp"

class EmployeeParser
{
public:
    void parseEmployees(table employeesTable)
    {
        for(const std::vector<std::string>& row : employeesTable)
        {
            if ( std::any_of(employees.begin(), employees.end(), [ row ](Employee& e){
                return e.getName() == row[0];
            }) )
            {
                std::cout << "juz jest" << std::endl;
            }
            else
            {
                Employee employee { row[0] }; // name
                
                std::string start = row[1], finish = row[2];
                start.replace(start.find(":"), 1, ".");
                finish.replace(finish.find(":"), 1, ".");
                employees.push_back(employee);
            }

        }
    }

private:
    std::vector<Employee> employees;
};