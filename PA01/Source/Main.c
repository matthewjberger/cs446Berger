#include "Utilities.h"

int main(int argc, char *argv[])
{
    // We expect a single argument
    if(!HandleArgs(argc, 1)) return 0;

    // Open the file and establish a buffer
    char const* filename = argv[1];
    FILE* file = fopen(filename, "r");
    char lineBuffer[255];

    // Read the file line by line
    while (fgets(lineBuffer, sizeof(lineBuffer), file))
    {
        // Break the line up into tokens by whitespace
        char* token;
        token = strtok(lineBuffer, " ");
        while(token != NULL)
        {
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }
    }

    // Close the file
    fclose(file);
}
