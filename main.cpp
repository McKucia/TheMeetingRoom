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
    parser.parseEmployees(CsvTable);

    return 0;
}