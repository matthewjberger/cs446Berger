#include "Simulator.h"
using namespace std;

// Utility Functions
int main(const int argc, const char* argv[])
{
    string configurationFile;

    // Ensure a single argument was provided
    if( argc != 2 )
    {
        cerr << "Error! A configuration file name must be provided." << endl;
        return EXIT_FAILURE;
    }

    // Ensure the file exists
    configurationFile = argv[1];
    if(access(configurationFile.c_str(), F_OK) == -1)
    {
        cerr << "Configuration file does not exist!" << endl;
        return EXIT_FAILURE;
    }

    try
    {
        // Run the simulator
        Simulator simulator(configurationFile);
        simulator.run();
    }

    catch( const std::runtime_error& ex )
    {
        cerr << ex.what() << endl;
        cerr << "Aborting program..."<< endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
