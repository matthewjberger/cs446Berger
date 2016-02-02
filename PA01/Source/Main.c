#include "Utilities.h"

int main(int argc, char *argv[])
{
    // We expect a single argument
    if(!HandleArgs(argc, 1)) return 0;

    // Read in data from the file
    char const* filename = argv[1];
    FILE* file = fopen(filename, "r");
    char lineBuffer[255];
    while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
        printf("%s", lineBuffer);
    }
    fclose(file);
}
