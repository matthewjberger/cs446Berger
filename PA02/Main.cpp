#include "Configuration.h"
#include "SimpleTimer.h"
#include <unistd.h>
using namespace std;

// Utility Functions
bool HandleArgs(int argc, int numberOfArgs);

int main(int argc, char* argv[])
{
    string configurationFile = argv[1];

    // Ensure a single argument
    if(!HandleArgs(argc, 1))
    {
        return 0;
    }

    cout << "Creating config" << endl;

    // Ensure the file exists
    if(access(configurationFile.c_str(), F_OK) == -1)
    {
        cout << "Configuration file does not exist!" << endl;
        return 0;
    }

    cout << "Creating config" << endl;
    ConfigurationData configurationData(configurationFile);
    configurationData.PrintData();

    return 0;
}

bool HandleArgs(int argc, int numberOfArgs)
{
    /* argc is the number of arguments passed
       argv contains the arguments
       argv[0] is the name of the program,
       so with no arguments, argc is equal to 1. */
    if( argc < (numberOfArgs + 1) )
    {
        cout << "Not enough arguments." << endl;
        return false;
    }
    else if( argc > (numberOfArgs + 1) )
    {
        cout << "Too many arguments received." << endl;
        return false;
    }

    return true;
}
