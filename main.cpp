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
    std::vector<std::string> allEmployees;
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

    float totalOverlapTime = 0.0f, totalTime = 0.0f;
    std::vector<Overlap> overlaps;
    for(int i = 0; i < records.size(); i++)
    {
        totalTime += records[i].workingPeriod.getTime();

        for(int j = i + 1; j < records.size(); j++)
        {
            std::string firstName = records[i].name;
            std::string secondName = records[j].name;

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
                        allExistedOverlaps.at(k)->names.push_back(firstName);
                        allExistedOverlaps.at(k)->names.push_back(secondName);

                        if(!std::any_of(allExistedOverlaps.at(k)->allEmployees.begin(), allExistedOverlaps.at(k)->allEmployees.end(), [firstName](std::string& e) { 
                            return e == firstName;
                        }))
                        {
                            allExistedOverlaps.at(k)->allEmployees.push_back(firstName);
                        }
                        if(!std::any_of(allExistedOverlaps.at(k)->allEmployees.begin(), allExistedOverlaps.at(k)->allEmployees.end(), [secondName](std::string& e) { 
                            return e == secondName;
                        }))
                        {
                            allExistedOverlaps.at(k)->allEmployees.push_back(secondName);
                        }
                        
                        if(k + 1 < allExistedOverlaps.size())
                        {
                            const float secondStart = allExistedOverlaps.at(k + 1)->overlapPeriod.start;
                            const float firstfinish = allExistedOverlaps.at(k)->overlapPeriod.finish;

                            if(secondStart != firstfinish)
                            {
                                WorkingPeriod  betweenOverlap(firstfinish, secondStart);
                                Overlap overlap(firstName, secondName, betweenOverlap, secondStart - firstfinish);

                                if(!std::any_of(overlap.allEmployees.begin(), overlap.allEmployees.end(), [firstName](std::string& e) { 
                                    return e == firstName;
                                }))
                                {
                                    overlap.allEmployees.push_back(firstName);
                                };

                                if(!std::any_of(overlap.allEmployees.begin(), overlap.allEmployees.end(), [secondName](std::string& e) { 
                                    return e == secondName;
                                }))
                                {
                                    overlap.allEmployees.push_back(secondName);
                                };

                                tmpOverlaps.push_back(overlap);
                                // totalOverlapTime += secondStart - firstfinish;
                            }
                        }
                    }
                    
                    for(Overlap tmpO : tmpOverlaps)
                    {
                        bool found = std::any_of(overlaps.begin(), overlaps.end(), [tmpO](Overlap& o) { 
                            return (o.overlapPeriod.start == tmpO.overlapPeriod.start &&
                                    o.overlapPeriod.finish == tmpO.overlapPeriod.finish);
                        });
                        if(!found && tmpO.overlapPeriod.start < tmpO.overlapPeriod.finish)
                        {
                            overlaps.push_back(tmpO);
                        }
                    }
                
                }
                else
                {
                    Overlap overlap(firstName, secondName, overlapPeriod, overlapTime);

                    overlap.allEmployees.push_back(firstName);
                    overlap.allEmployees.push_back(secondName);

                    overlaps.push_back(overlap);
                }
                 
            }
        }
    }

    std::cout << std::endl;
    for(Overlap o : overlaps)
    {
        totalOverlapTime += (o.allEmployees.size() - 1) * o.overlapTime;
        std::cout << "From " << o.overlapPeriod.start << " to " << o.overlapPeriod.finish << " - " << o.allEmployees.size() << " employees" << std::endl;
        for(std::string name : o.allEmployees)
        {
            std::cout << name << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "The total use time of the room: " << totalTime - totalOverlapTime << std::endl;

    Overlap maxOverlap("", "", WorkingPeriod(0.0f, 0.0f), 0.0f);
    for(Overlap o : overlaps)
    {
        if(o.overlapTime > maxOverlap.overlapTime)
            maxOverlap = o;
    }

    std::cout << std::endl;
    std::cout << "The time frame in which there have been more employees: " << maxOverlap.overlapPeriod.start << " " << maxOverlap.overlapPeriod.finish << std::endl;

    return 0;
}