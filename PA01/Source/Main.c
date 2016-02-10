#include "Utilities.h"

#define MAX_ROWS 30
#define MAX_COLUMNS 255

#define PrintTabs(numTabs) for(int count = 0; count < numTabs; count++){ printf("    "); }

int main(int argc, char *argv[])
{
    // We expect a single argument
    if(!HandleArgs(argc, 1)) return 0;

    // Open the file and establish a buffer
    const char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    char lineBuffer[MAX_COLUMNS];

    // Delimiters for tokenizing by whitespace
    char *delimiters=" \n\t\r\v\f";

    // Number of tabs for formatting
    int numberOfTabs = 0;

    // Get the first line of the file
    if(fgets(lineBuffer, sizeof(lineBuffer), file))
    {
        // FIRST TIER
        // Parents
        char* parent_1 = strtok(lineBuffer, delimiters);
        if(parent_1 == NULL) return 0;

        char* parent_2 = strtok(NULL, delimiters);
        if(parent_2 == NULL) return 0;

        printf("%s(0)-%s\n", parent_1, parent_2);

        // Fork and print children in new process
        pid_t pid = fork();
        numberOfTabs++;

        if (pid != 0)
        {
            // SECOND TIER
            char* childName = strtok(NULL, delimiters);
            while(childName != NULL)
            {
                // Store the current position to return to after parsing the rest of the file
                fpos_t currentPosition;
                fgetpos(file, &currentPosition);

                // Check the rest of the file for possible children
                char childLineBuffer[MAX_COLUMNS];
                bool foundSpouse = false;
                while(fgets(childLineBuffer, sizeof(childLineBuffer), file))
                {
                    // Ignore blank lines
                    if(strcmp(childLineBuffer, "") == 0) continue;

                    char* parent_1 = strtok(childLineBuffer, delimiters);
                    if(parent_1 == NULL) return 0;

                    char* parent_2 = strtok(NULL, delimiters);
                    if(parent_2 == NULL) return 0;

                    // If the child's name was found in the first or second word
                    // of any line later in the file
                    if( (strcmp(parent_1, childName) == 0) || (strcmp(parent_2, childName) == 0))
                    {
                        foundSpouse = true;
                        PrintTabs(numberOfTabs);
                        printf("%s(%i)-%s\n", parent_1, pid, parent_2);

                        // Fork on each child
                        pid = fork();
                        numberOfTabs++;
                        if(pid != 0)
                        {
                            PrintTabs(numberOfTabs);
                            // THIRD TIER
                            printf("(%i)I'm a third tier process mommy!\n", pid);
                            return 0;
                        }

                        break;
                    }
                }

                // If spouse was found, simply print the child's name
                if(!foundSpouse)
                    printf("%s(%i)\n", childName, pid);

                // Reset the stream indicator
                fsetpos(file, &currentPosition);

                // Get the next child
                childName = strtok(NULL, delimiters);
            }
        }
        else
        {
            // Wait for child processes to finish
            waitpid(pid, NULL, 0);
        }
    }

    // Close the file
    fclose(file);
}
