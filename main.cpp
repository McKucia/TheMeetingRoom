#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "CSVReader.hpp"
#include "WorkingPeriod.hpp"
#include "EmployeeParser.hpp"

int main()
{
    std::ifstream file("employee_meeting_time.csv");
    CSVReader reader;
    EmployeeParser parser;

    table CsvTable = reader.readAll(file, true);
    std::vector<Employee> employees = parser.parseEmployees(CsvTable);

    std::cout << "Total number of employees: " << employees.size() << std::endl;

    return 0;
}