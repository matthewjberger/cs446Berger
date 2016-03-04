#include "Simulator.h"
#include "SimpleTimer.h"
#include <unistd.h>

using namespace std;

// Utility Functions
int main(const int argc, const char* argv[])
{
    string configurationFile = argv[1];

    // Ensure a single argument was provided
    if( argc != 2 )
    {
        cerr << "Error! A configuration file name must be provided." << endl;
        return EXIT_FAILURE;
    }

    // Ensure the file exists
    if(access(configurationFile.c_str(), F_OK) == -1)
    {
        cerr << "Configuration file does not exist!" << endl;
        return EXIT_FAILURE;
    }

    try
    {
    }

    catch( ... )
    {
        std::cerr << "Error! Fatal exception occurred. Aborting program." << endl;
    }

    return EXIT_SUCCESS;
}
