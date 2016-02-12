#include "Utilities.h"

#define MAX_ROWS 30
#define MAX_COLUMNS 255
#define MAX_CHILDREN 100
#define MAX_TIERS 100
#define FIRST_TIER 0
#define FIRST_INDENT 0

#define PrintTabs(numTabs) for(int count = 0; count < numTabs; count++){ printf("    "); }
#define STRCPY(dst,src) strcpy(dst=malloc(strlen(src)+1), src) // This will allocate memory before a strcpy

typedef struct {
    char *parent_1;
    char *parent_2;
    char *children[MAX_CHILDREN];
    bool hasSpouse;
} FamilyTier;

const char *delimiters=" \n\t\r\v\f"; // Delimiters for tokenizing by whitespace

bool BuildTier(FamilyTier *tier, char* line);
void FindChildren(FamilyTier familyTiers[MAX_TIERS], int currentTier, int numberOfTiers, int numberOfTabs);

int main(int argc, char *argv[])
{
    // Variables
    FamilyTier familyTiers[MAX_TIERS];
    char* filename;
    FILE* file;
    char lineBuffer[MAX_COLUMNS];
    int tierCounter = -1;

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
        // Only increment after successful line read
        tierCounter++;

        // Ignore blank lines in file
        if(strcmp(lineBuffer,"\n") == 0) continue;

        // Fill the struct with data
        if(!BuildTier(&familyTiers[tierCounter], lineBuffer))
            printf("Error reading file!\n");

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
    STRCPY(tier->parent_1, parent_1);

    parent_2 = strtok(NULL, delimiters);
    if(parent_2 == NULL) return false;
    STRCPY(tier->parent_2, parent_2);

    childName = strtok(NULL, delimiters);
    while(childName != NULL)
    {
        // Add the child name to the current family tier
        STRCPY(tier->children[childCounter], childName);

        // Get the next child
        childName = strtok(NULL, delimiters);
        childCounter++;
    }

    return true;
}

void FindChildren(FamilyTier familyTiers[MAX_TIERS], int currentTier, int numberOfTiers, int numberOfTabs)
{
    // Variables

    // Print married name

    // Increment numTabs

    // Iterate through all children

        // Fork

        // If pid == 0

            // Iterate through the rest of the tiers

                // If the parent_1 or parent_2 matches the child's name

                    // recurse

                // If not
                    // Print just it's name
                    // return

        // else wait
}
