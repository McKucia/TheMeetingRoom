#pragma once

struct WorkingPeriod
{
    WorkingPeriod(float _start, float _finish) : start(_start), finish(_finish) {}
    const float getTime() const { return finish - start; }
    float start, finish;
};

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