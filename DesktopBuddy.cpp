#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <sys/stat.h>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "Tasklist.h"
using namespace std;

string get_current_dir() //returns the current working directory
{
   char buff[FILENAME_MAX];
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}

inline bool fileExists(const string& name) //helper function to check if file exists given filepath
{
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

void displayIntro() //Function to display aesthetic Header
{
    int slashValue = 92;
    //cout << endl << endl;
    cout<< " _____            _    _                ____            _     _       " << endl
        << "|  __ "<< char(slashValue) <<"          | |  | |              |  _ "<< char(slashValue) <<"          | |   | |      " << endl
        << "| |  | | ___  ___| | _| |_ ___  _ __   | |_) |_   _  __| | __| |_   _ " << endl
        << "| |  | |/ _ "<< char(slashValue) <<"/ __| |/ / __/ _ "<< char(slashValue) <<"| '_ "<< char(slashValue) <<"  |  _ <| | | |/ _` |/ _` | | | |" << endl
        << "| |__| |  __/"<< char(slashValue) <<"__ "<< char(slashValue) <<"   <| || (_) | |_) | | |_) | |_| | (_| | (_| | |_| |" << endl
        << "|_____/ "<< char(slashValue) <<"___||___/_|"<< char(slashValue) <<"_"<< char(slashValue) <<""<< char(slashValue) <<"__"<< char(slashValue) <<"___/| .__/  |____/ "<< char(slashValue) <<"__,_|"<< char(slashValue) <<"__,_|"<< char(slashValue) <<"__,_|"<< char(slashValue) <<"__, |" << endl
        << "                               | |                               __/ |" << endl
        << "                               |_|                              |___/ " << endl;
    
}

pair<int, int> getMaxString(vector<tuple<string, Date, string>> input) //return the lengths of the longest strings in a string, Date, string tuple
{
    auto maxes = make_pair<int, int>(6,6); //minimum column width of 5, can be changed to 0 for multipurpose getMax function

    for(tuple<string, Date, string> task : input) //store length of the longest task name
    {
        if(get<0>(task).length() > get<0>(maxes))
        {
            maxes.first = get<0>(task).length();
        }
        
    }
    cout << maxes.first << endl;

    for(tuple<string, Date, string> task : input) //store length of the longest task type
    {
        if(get<2>(task).length() > get<1>(maxes))
        {
            maxes.second = get<2>(task).length();
        }   
    }
    cout << maxes.second << endl;
    return maxes;
}

void printChar(int x, const char* c) //helper method to print a character x number of times in console
{
    for(int i=0; i < x+1; i++)
    {
        cout << c;
    }
} 

void displayTasks(Tasklist &currentTasklist)
{
    auto maxes = getMaxString(currentTasklist.getTasks()); //pair of ints showing 
    int totalWidth = maxes.first+4 + 11 + maxes.second+4;
    cout << " ";
    printChar(totalWidth, "_"); //print length of all 3 columns

    
    cout << endl << "|"; 
    printChar((maxes.first-2)/2, " "); 
    cout << "Task"; printChar((maxes.first-2)/2, " "); 
    cout << "|";
    
    cout << "   Date   |";

    printChar((maxes.second-2)/2, " ");
    cout << "Type"; printChar((maxes.second-2)/2, " ");
    cout << "|" << endl;

    printChar(totalWidth, "_");
    cout << endl;


    for(tuple<string, Date, string> task : currentTasklist.getTasks())
    {
        cout << "| " << get<0>(task) << " |" << endl;      
    }
}

Tasklist readFromCSV() //Function to populate a tasklist based on CSV contents
{
    ifstream tasklistFile;
    tasklistFile.open("tasks.csv", ios::in);
    Tasklist tasklist = Tasklist();

    vector<string> row;
    string line, field;
    
    while (getline(tasklistFile, line))
    {
        stringstream ss(line);
        while(getline(ss, field, ','))
        {
            row.push_back(field);
        }
        //CSV format = name, day, month, year, task type
        Date date = Date(stoi(row[1]), stoi(row[2]), stoi(row[3]));
        tasklist.addTask(row[0], date, row[4]);

        row.clear();
    }
    tasklistFile.close();
    return tasklist;   
}

void tasklistMain() //Main driver code for tasklist functionality
{
    char input;
    Tasklist tasklist = readFromCSV(); //populate tasklist from current CSV
    displayIntro(); //Displays aesthetic header
    displayTasks(tasklist);
    while(input != 'e' || input != 'E') //e to exit
    {   
        cin >> input;
    }
}

int main()
{
    system("Color 03");
    int slashValue = 92;
    string testfile = get_current_dir() + char(slashValue) + "tasks.csv";
    if(!fileExists(testfile)) //create a new csv file to store tasks in current directory
    {
        ofstream tasklistFile;
        tasklistFile.open("tasks.csv", ios::out | ios::app);
        system("Color 0E");
        cout << "Welcome to your new Desktop Buddy! - An empty tasklist has been created for you";
        tasklistFile.close();
    }
    else
    {
        cout << "Welcome back Buddy!" << endl;
    }

    tasklistMain();
    return 0;
}