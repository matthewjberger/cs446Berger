#include "Utilities.h"

bool HandleArgs(int argc, int numberOfArgs)
{
    /* argc is the number of arguments passed
       argv contains the arguments
       argv[0] is the name of the program,
       so with no arguments, argc is equal to 1. */
    if( argc < (numberOfArgs + 1) )
    {
        printf("Not enough arguments.\n");
        return false;
    }
    else if( argc > (numberOfArgs + 1) )
    {
        printf("Too many arguments received.\n");
        return false;
    }

    return true;
}
