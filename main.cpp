#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "CSVReader.hpp"
#include "WorkingPeriod.hpp"
#include "EmployeeParser.hpp"

bool overlapped(const WorkingPeriod& w1, const WorkingPeriod& w2){
    return std::max(w1.start, w2.start) < std::min(w1.finish, w2.finish);
}

struct Record
{
    Record(std::string _name, WorkingPeriod _workingPeriod) :
        name(_name), workingPeriod(_workingPeriod) {}

    WorkingPeriod workingPeriod;
    std::string name;
};

struct Overlap
{
    Overlap(std::string _name1,  std::string _name2, WorkingPeriod _overlapPeriod, float _overlapTime) :
        overlapPeriod(_overlapPeriod), overlapTime(_overlapTime) 
    {
        names.push_back(_name1);
        names.push_back(_name2);
    }

    WorkingPeriod overlapPeriod;
    float overlapTime;
    std::vector<std::string> names;
};

int main()
{
    std::ifstream file("employee_meeting_time.csv");
    CSVReader reader;
    EmployeeParser parser;

    table CsvTable = reader.readAll(file, true);
    std::vector<Employee> employees = parser.parseEmployees(CsvTable);

    std::cout << "Total number of employees: " << employees.size() << std::endl;

    std::vector<Record> records;
    for(const Employee& e : employees)
    {
        for(WorkingPeriod wp : e.getWorkingPeriods())
        {
            Record record(e.getName(), wp);
            records.push_back(record);
        }
    }

    float totalOverlapTime = 0, totalTime = 0;
    std::vector<Overlap> overlaps;
    for(int i = 0; i < records.size(); i++)
    {
        totalTime += records[i].workingPeriod.getTime();
        for(int j = i + 1; j < records.size(); j++)
        {
            if(overlapped(records[i].workingPeriod, records[j].workingPeriod))
            {
                float min = std::min(records[i].workingPeriod.finish, records[j].workingPeriod.finish);
                float max = std::max(records[i].workingPeriod.start, records[j].workingPeriod.start);
                float overlapTime = min - max;

                WorkingPeriod overlapPeriod(max, min);

                if(std::any_of(overlaps.begin(), overlaps.end(), [overlapPeriod](Overlap& o){
                    return overlapped(o.overlapPeriod, overlapPeriod);
                }))
                {
                    std::vector<Overlap*> allExistedOverlaps;

                    auto it = overlaps.begin();
                    while ((it = std::find_if(it, overlaps.end(), [overlapPeriod](Overlap& o) { 
                        return overlapped(o.overlapPeriod, overlapPeriod);
                    })) != overlaps.end())
                    {
                        allExistedOverlaps.push_back(&*it);
                        it++;
                    }
                    
                    std::vector<Overlap> tmpOverlaps;
                    for(int k = 0; k < allExistedOverlaps.size(); k++)
                    {
                        allExistedOverlaps.at(k)->names.push_back(records[i].name);
                        allExistedOverlaps.at(k)->names.push_back(records[j].name);

                        if(k + 1 < allExistedOverlaps.size())
                        {
                            const float secondStart = allExistedOverlaps.at(k + 1)->overlapPeriod.start;
                            const float firstfinish = allExistedOverlaps.at(k)->overlapPeriod.finish;

                            if(secondStart != firstfinish)
                            {
                                WorkingPeriod  betweenOverlap(firstfinish, secondStart);
                                Overlap overlap(records[i].name, records[j].name, betweenOverlap, secondStart - firstfinish);
                                tmpOverlaps.push_back(overlap);
                                // totalOverlapTime += secondStart - firstfinish;
                            }
                        }
                    }

                    overlaps.insert(overlaps.end(), tmpOverlaps.begin(), tmpOverlaps.end());
                    totalOverlapTime += overlapTime;
                }
                else
                {
                    Overlap overlap(records[i].name, records[j].name, overlapPeriod, overlapTime);
                    overlaps.push_back(overlap);
                    totalOverlapTime += overlapTime;
                }
                 
            }
        }
    }

    std::cout << "The total use time of the room: " << totalTime - totalOverlapTime << std::endl;

    return 0;
}