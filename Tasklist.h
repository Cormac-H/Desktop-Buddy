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

    string toString(){return to_string(day) + "/" + to_string(month) + "/" + to_string(year);}
};

class Tasklist //Class for a tasklist
{
private:
    std::vector<tuple<std::string, Date, std::string>> tasks; //Each task has a name, due date, and type

public:
    // constructor
    Tasklist(){}

    std::vector<tuple<std::string, Date, std::string>> getTasks(){return tasks;}

    std::string getTaskName(int i) //return name of i'th task
    { 
        return std::get<0>(tasks[i-1]); 
    }

    Date getTaskDate(int i) //return date of i'th task
    {
        return std::get<1>(tasks[i-1]); 
    }

    std::string getTaskType(int i) //return type of i'th task
    { 
        return std::get<2>(tasks[i-1]); 
    }

    void addTask(std::string name, Date date, std::string type) //add task to tasklist
    {
        tasks.push_back(make_tuple(name, date, type));
    } 

    void removeTask(int i){};

    void deleteAllTasks(){};


};



#endif

