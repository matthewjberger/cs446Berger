#include "Utilities.h"

#define MAX_ROWS 30
#define MAX_COLUMNS 255
#define MAX_CHILDREN 100
#define MAX_TIERS 100

#define PrintTabs(numTabs) for(int count = 0; count < numTabs; count++){ printf("    "); }

typedef struct {
    char *parent_1;
    char *parent_2;
    char *children[MAX_CHILDREN];
    bool hasSpouse;
} FamilyTier;

const char *delimiters=" \n\t\r\v\f"; // Delimiters for tokenizing by whitespace

bool BuildTier(FamilyTier *tier, char* line);

int main(int argc, char *argv[])
{
    // Variables
    FamilyTier familyTiers[MAX_TIERS];
    char* filename;
    FILE* file;
    char lineBuffer[MAX_COLUMNS];
    int tierCounter = 0;

    // We expect a single argument
    if(!HandleArgs(argc, 1)) return 0;

    // Open the file and establish a buffer
    filename = argv[1];
    if(access(filename, F_OK) == -1)
    {
        printf("Input file does not exist!\n");
        return 0;
    }
    file = fopen(filename, "r");

    // Get the first line of the file
    while(fgets(lineBuffer, sizeof(lineBuffer), file))
    {
        // Ignore blank lines in file
        if(strcmp(lineBuffer,"\n") == 0) continue;

        // Fill the struct with data
        if(!BuildTier(&familyTiers[tierCounter], lineBuffer))
            printf("Error reading file!\n");

        tierCounter++;
    }

    // Close the file
    fclose(file);
}

bool BuildTier(FamilyTier *tier, char* line)
{
    // Variables
    char* parent_1;
    char* parent_2;
    int childCounter = 0;
    char* childName;

    // Parents
    parent_1 = strtok(line, delimiters);
    if(parent_1 == NULL) return false;
    tier->parent_1 = parent_1;

    parent_2 = strtok(NULL, delimiters);
    if(parent_2 == NULL) return false;
    tier->parent_2 = parent_2;

printf("%s-%s\n", tier->parent_1, tier->parent_2);

    childName = strtok(NULL, delimiters);
    while(childName != NULL)
    {
        // Add the child name to the current family tier
        tier->children[childCounter] = childName;
printf("%s\n", tier->children[childCounter]);

        // Get the next child
        childName = strtok(NULL, delimiters);
        childCounter++;
    }

    return true;
}
