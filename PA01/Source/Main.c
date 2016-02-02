#include <stdio.h>

// Function Signatures
void HandleArgs(int argc, char* argv[]);

// Main program
int main(int argc, char *argv[])
{
    HandleArgs(argc, argv);
}

// Function Implementations
void HandleArgs(int argc, char* argv[])
{
    /* argc is the number of arguments passed
       argv contains the arguments
       argv[0] is the name of the program,
       so with no arguments, argc is equal to 1. */
    if( argc < 2 )
        printf("The input file name is a required argument.\n");
    else if(argc > 2)
        printf("Too many arguments received.\n");
    else
        printf("The argument supplied is %s\n", argv[1]);
}
