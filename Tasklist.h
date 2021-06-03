#ifndef TASKLIST
#define TASKLIST
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;
class Date //class for dd-mm-yyyy value
{
private:
    int day;
    int month;
    int year;

public:

    Date(int day, int month, int year)
    {
        this->day = day;
        this->month = month;
        this->year = year;
    }

    int getDay(){return day;}

    int getMonth(){return month;}

    int getYear(){return year;}
};

class Tasklist //Class for a tasklist
{
private:
    std::vector<tuple<std::string, Date, std::string>> tasks; //Each task has a name, due date, and type

public:
    // constructor
    Tasklist(){}

    std::string getTaskName(int i)
    { 
        return std::get<0>(this->tasks[i+1]); 
    }

    Date getTaskDate(int i)
    {
        return std::get<1>(this->tasks[i+1]); 
    }

    std::string getTaskType(int i)
    { 
        return std::get<2>(this->tasks[i+1]); 
    }

    void addTask(std::string name, Date date, std::string type){}

    void removeTask(int i){};

    void deleteAllTasks(){};


};



#endif

