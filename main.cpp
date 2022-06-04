#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "CSVReader.hpp"

int main()
{
    std::ifstream file("employee_meeting_time.csv");
    CSVReader row;

    table CsvTable = row.readAll(file, true);
    
    return 0;
}