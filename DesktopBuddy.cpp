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
#include <cctype>
#include <locale>
#include <stdlib.h>
#include "Tasklist.h"
using namespace std;

string get_current_dir() //returns the current working directory
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    string current_working_dir(buff);
    return current_working_dir;
}

inline bool fileExists(const string &name) //helper function to check if file exists given filepath
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void displayIntro() //Function to display aesthetic Header
{
    int slashValue = 92;
    cout << endl;
    cout << " _____            _    _                ____            _     _       " << endl
         << "|  __ " << char(slashValue) << "          | |  | |              |  _ " << char(slashValue) << "          | |   | |      " << endl
         << "| |  | | ___  ___| | _| |_ ___  _ __   | |_) |_   _  __| | __| |_   _ " << endl
         << "| |  | |/ _ " << char(slashValue) << "/ __| |/ / __/ _ " << char(slashValue) << "| '_ " << char(slashValue) << "  |  _ <| | | |/ _` |/ _` | | | |" << endl
         << "| |__| |  __/" << char(slashValue) << "__ " << char(slashValue) << "   <| || (_) | |_) | | |_) | |_| | (_| | (_| | |_| |" << endl
         << "|_____/ " << char(slashValue) << "___||___/_|" << char(slashValue) << "_" << char(slashValue) << "" << char(slashValue) << "__" << char(slashValue) << "___/| .__/  |____/ " << char(slashValue) << "__,_|" << char(slashValue) << "__,_|" << char(slashValue) << "__,_|" << char(slashValue) << "__, |" << endl
         << "                               | |                               __/ |" << endl
         << "                               |_|                              |___/ " << endl;
}

pair<int, int> getMaxString(vector<tuple<string, Date, string>> input) //return the lengths of the longest strings in a string, Date, string tuple
{
    auto maxes = make_pair<int, int>(6, 6); //minimum column width of 5, can be changed to 0 for multipurpose getMax function

    for (tuple<string, Date, string> task : input) //store length of the longest task name
    {
        if (get<0>(task).length() > get<0>(maxes))
        {
            maxes.first = get<0>(task).length();
        }
    }

    for (tuple<string, Date, string> task : input) //store length of the longest task type
    {
        if (get<2>(task).length() > get<1>(maxes))
        {
            maxes.second = get<2>(task).length();
        }
    }

    return maxes;
}

void printChar(int x, const char *c) //helper method to print a character x number of times in console
{
    for (int i = 0; i < x + 1; i++)
    {
        cout << c;
    }
}

void displaySimpleOptions() //Simple helper function to remind the user options are available
{
    cout << "Enter O to view Options: ";
}

void displayTasks(Tasklist &currentTasklist)
{
    auto maxes = getMaxString(currentTasklist.getTasks()); //pair of ints showing
    int totalWidth = maxes.first + 4 + 13 + maxes.second + 4;
    cout << " ";
    printChar(totalWidth, "_"); //print length of all 3 columns

    cout << endl
         << "|"; //print task column
    printChar((maxes.first - 2) / 2, " ");
    cout << "Task";
    printChar((maxes.first - 2) / 2, " ");
    if ((maxes.first - 2) % 2 != 0)
    {
        cout << " ";
    }
    cout << "|";

    cout << "    Date    |"; //print date column

    printChar((maxes.second - 2) / 2, " "); //print type column
    cout << "Type";
    printChar((maxes.second - 2) / 2, " ");
    if ((maxes.second - 2) % 2 != 0)
    {
        cout << " ";
    }
    cout << "|" << endl
         << "|";

    printChar(maxes.first + 3, "-"); //print dashed border to seperate tasks
    cout << "|";

    printChar(11, "-");
    cout << "|";

    printChar(maxes.second + 3, "-");
    cout << "|" << endl;

    for (tuple<string, Date, string> task : currentTasklist.getTasks()) //print each task
    {
        cout << "| " << get<0>(task);
        printChar((maxes.first + 2) - get<0>(task).length(), " "); //format task name
        cout << "|";

        if (get<1>(task).toString().length() == 10) //format task date
        {
            cout << " " <<  get<1>(task).toString() << " ";
        }
        else if(get<1>(task).toString().length() == 8)
        {
            cout << "  " << get<1>(task).toString() << "  ";
        }
        else
        {
            cout << "  " << get<1>(task).toString() << " ";
        }
        cout << "|";

        cout << " " << get<2>(task); //format task type
        printChar((maxes.second + 2) - get<2>(task).length(), " ");
        cout << "|" << endl;
    }

    printChar(totalWidth + 2, "-"); //print final border
    cout << endl;
    displaySimpleOptions();
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
        while (getline(ss, field, ','))
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



void displayAllOptions() //Function to display all available options
{
    cout << "Options List: " << endl;
    cout << "   - 'N' to add new task" << endl;
    cout << "   - 'D' to delete a task" << endl;
    cout << "   - 'S' to sort tasks" << endl;
    cout << "   - 'C' to complete a task" << endl;
    cout << "   - 'V' to view tasks" << endl;
}

bool isNumber(const std::string &s) //helper function to check if a string is a number
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool isValidDate(int day, int month) //helper function to check if a day/month combo is valid
{
    if(day <= 0 || day > 31 || month <= 0 || month > 12)
    {
        return false;
    }
    else if (month == 9 || month == 4 || month == 6  || month == 11 )
    {
        if(day > 30)
        {
            return false;
        }
    }
    else if ((month == 2) && (day > 28))
    {
        return false;
    }
    return true;
}

void writeToCSV(Tasklist tasklist) //Write a tasklist object to a CSV file
{
    ofstream tasklistFile;
    tasklistFile.open("tasks.csv", std::ofstream::out | std::ofstream::trunc); //trunc to delete everything inside the file
    
    for(tuple<string, Date, string> task : tasklist.getTasks()) //write entire tasklist to CSV file
    {
        tasklistFile << get<0>(task) << ","; //task name
        tasklistFile << get<1>(task).getDay() << ","; //day, month, year
        tasklistFile << get<1>(task).getMonth() << ",";
        tasklistFile << get<1>(task).getYear() << ",";
        tasklistFile << get<2>(task); //task type
        tasklistFile << endl;
    }
    tasklistFile.close();
}

Tasklist addTask() //Function to add a task to the current tasklist and update the CSV/displayed info
{
    string taskName, taskType, temp;
    int day, month, year;
    bool validDate = false;

    cout << "Task Name: ";
    cin.ignore(1,'\n');
    getline(cin, taskName); //Obtain a task name

    while (!validDate) //validate the date
    {
        while (temp == "" || !isNumber(temp))
        {
            cout << "Enter the day it is due (0-31): "; //Obtain a valid day of the month
            cin >> temp;

            if (cin.fail())
            {
                cout << endl
                     << "Please enter a date from 0-31" << endl;
                cin.clear();
            }
        }
        day = stoi(temp);
        temp = "";

        while (temp == "" || !isNumber(temp))
        {
            cout << "Enter the month it is due (1-12): "; //Obtain a valid month
            cin >> temp;

            if (cin.fail())
            {
                cout << endl
                     << "Please enter a month from 1-12" << endl;
                cin.clear();
            }
        }
        month = stoi(temp);
        temp = "";

        validDate = isValidDate(day, month);
        if(!validDate)
        {
            cout << "That day/month combo is not a valid calendar date or it's a leap year which i was too lazy to validate" << endl;
        }
    }

    while (temp == "" || !isNumber(temp))
    {
        cout << "Enter the year it is due: "; //Obtain a valid year
        cin >> temp;

        if (cin.fail())
        {
            cout << endl
                 << "Please enter a valid year (minimum 4 digits)" << endl;
            cin.clear();
        }
    }

    year = stoi(temp);
    temp = "";

    cout << "Task Type: ";
    cin.ignore(1,'\n');
    getline(cin, taskType); //Obtain a task name

    cout << "Your task has been added buddy!" << endl;
    Date date = Date(day, month, year);
    Tasklist tasklist = readFromCSV(); //grab the current tasklist

    tasklist.addTask(taskName, date, taskType); //add a task to it given the information
    
    writeToCSV(tasklist); //write the task to the CSV file and update the view and tasklist object
    displayTasks(tasklist);
    return tasklist;
}

static inline void ltrim(std::string &s) //trim start of string
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}


static inline void rtrim(std::string &s) { //trim end of string
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) { //trim both ends of a string
    ltrim(s);
    rtrim(s);
}

int caseInsensitiveMatch(string s1, string s2) { //helper function to perform a case-insensitive match
   //convert s1 and s2 into lower case strings
   transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
   transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
   trim(s1);
   trim(s2);
   if(s1.compare(s2) == 0)
      return 1; //The strings are same
   return 0; //not matched
}

Tasklist deleteTask(Tasklist &tasklist)
{
    string taskName;
    bool removed = false;
    cout << "What's the name of the task you'd like to delete: ";
    cin.ignore(1,'\n');
    getline(cin, taskName);

    for(int i = 0; i < tasklist.getTasks().size(); i++)
    {
        if(caseInsensitiveMatch(tasklist.getTaskName(i), taskName))
        {
            tasklist.removeTask(i); //if the i'th element matches the given task name, erase it
            removed = true;
        }
    }
    
    if(!removed)
    {
        cout << "Sorry we couldn't find your task!" << endl << endl;
    }
    else
    {
        cout << "Your task has been removed buddy!" << endl << endl;
    }

    writeToCSV(tasklist);
    displayTasks(tasklist);
    return tasklist;
}

void congratulate() //helper function to print aesthetic congratulations method
{
    cout << "                                 _         _       _   _                 " << endl;
    cout << "                                | |       | |     | | (_)                " << endl;
    cout << "  ___ ___  _ __   __ _ _ __ __ _| |_ _   _| | __ _| |_ _  ___  _ __  ___ "<< endl;
    cout << " / __/ _ \\| '_ \\ / _` | '__/ _` | __| | | | |/ _` | __| |/ _ \\| '_ \\/ __|"<< endl;
    cout << "| (_| (_) | | | | (_| | | | (_| | |_| |_| | | (_| | |_| | (_) | | | \\__ \\"<< endl;
    cout << " \\___\\___/|_| |_|\\__, |_|  \\__,_|\\__|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_|___/"<< endl;
    cout << "                  __/ |                                                  " << endl;
    cout << "                 |___/                                                   " << endl;

    cout << endl << "Great work buddy! Keep it up!!" << endl << endl;
}

Tasklist completeTask(Tasklist &tasklist)
{
    string taskName;
    bool removed = false;
    cout << "What's the name of the task you completed: ";
    cin.ignore(1,'\n');
    getline(cin, taskName);

    for(int i = 0; i < tasklist.getTasks().size(); i++)
    {
        if(caseInsensitiveMatch(tasklist.getTaskName(i), taskName))
        {
            tasklist.removeTask(i); //if the i'th element matches the given task name, erase it
            removed = true;
        }
    }
    
    if(!removed)
    {
        cout << "Sorry we couldn't find your task!" << endl << endl;
    }
    else
    {
        congratulate();
    }

    writeToCSV(tasklist);
    displayTasks(tasklist);
    return tasklist;
}

void tasklistMain() //Main driver code for tasklist functionality
{
    char input;
    displayIntro();  //Displays aesthetic header 
    Tasklist tasklist;
    ifstream tasklistFile;
    tasklistFile.open("tasks.csv", ios::in);
    string temp;
    getline(tasklistFile, temp);
    if(temp != "")
    {
        tasklist = readFromCSV(); //populate tasklist from current CSV
        displayTasks(tasklist); //Prints the tasklist object as a visual list
    }
    else
    {
        displaySimpleOptions();
    }
    
    while (input != 'e' && input != 'E') //Awaits user input e.g. adding or deleting tasks. E to exit
    {
        cin >> input;
        if (input == 'o' || input == 'O')
        {
            displayAllOptions();
            displaySimpleOptions();
        }
        else if (input == 'n' || input == 'N')
        {
            tasklist = addTask();
        }
        else if (input == 'd' || input == 'D')
        {
            tasklist = deleteTask(tasklist);
        }
        else if (input == 'c' || input == 'C')
        {
            completeTask(tasklist);
        }
        else if (input == 'v' || input == 'V')
        {
            displayTasks(tasklist);
        }
    }
    exit(EXIT_SUCCESS);
}

int main()
{
    system("Color 03");
    int slashValue = 92;
    string testfile = get_current_dir() + char(slashValue) + "tasks.csv";

    if (!fileExists(testfile)) //create a new csv file to store tasks in current directory
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