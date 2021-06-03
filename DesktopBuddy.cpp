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

void displayDefault()
{
    int slashValue = 92;
    cout << endl << endl;
    cout << " _____            _    _                ____            _     _       " << endl
        << "|  __ "<< char(slashValue) <<"          | |  | |              |  _ "<< char(slashValue) <<"          | |   | |      " << endl
        << "| |  | | ___  ___| | _| |_ ___  _ __   | |_) |_   _  __| | __| |_   _ " << endl
        << "| |  | |/ _ "<< char(slashValue) <<"/ __| |/ / __/ _ "<< char(slashValue) <<"| '_ "<< char(slashValue) <<"  |  _ <| | | |/ _` |/ _` | | | |" << endl
        << "| |__| |  __/"<< char(slashValue) <<"__ "<< char(slashValue) <<"   <| || (_) | |_) | | |_) | |_| | (_| | (_| | |_| |" << endl
        << "|_____/ "<< char(slashValue) <<"___||___/_|"<< char(slashValue) <<"_"<< char(slashValue) <<""<< char(slashValue) <<"__"<< char(slashValue) <<"___/| .__/  |____/ "<< char(slashValue) <<"__,_|"<< char(slashValue) <<"__,_|"<< char(slashValue) <<"__,_|"<< char(slashValue) <<"__, |" << endl
        << "                               | |                               __/ |" << endl
        << "                               |_|                              |___/ " << endl;
}

Tasklist readFromCSV()
{
    fstream tasklistFile;
    tasklistFile.open("tasks.csv", ios::in);
    Tasklist *tasklist = new Tasklist();

    vector<string> row;
    string temp, line, word;
    while(tasklistFile >> temp)
    {
        getline(tasklistFile, line);
        stringstream s(line);

        while(getline(s, word, char(',')))
        {
            row.push_back(word);
        }
        for(string t : row){
            cout << t << endl;
        }
        cout << endl << endl;
    }
}

void tasklistMain() //Main driver code for tasklist functionality
{
    char input;
    Tasklist tasklist = readFromCSV();
    readFromCSV();
    displayDefault();
    while(input != 'e' || input != 'E')
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
        cout << "Welcome back Buddy!";
    }

    tasklistMain();
    return 0;
}