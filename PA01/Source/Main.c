#include "Utilities.h"

#define MAX_ROWS 30
#define MAX_COLUMNS 255
#define MAX_CHILDREN 100
#define MAX_TIERS 100
#define FIRST_TIER 0
#define FIRST_INDENT 0

// This will print the appropriate number of tabs
#define PrintTabs(numTabs) for(int count = 0; count < numTabs; count++){ printf("    "); }

// This will allocate memory before a strcpy
// dst will be a pointer to a string, src is a c-style string to copy
// malloc() allocates enough memory for each chracter in the src, +1 for the null character
// then we strcpy like normal
#define STRCPY(dst,src) strcpy(dst=malloc(strlen(src)+1), src)

// There is one instance of this for every line in the file
typedef struct {
    char *parent_1;
    char *parent_2;
    char *children[MAX_CHILDREN];
    int numberOfChildren;
} FamilyTier;

// Delimiters for tokenizing by whitespace
const char *delimiters=" \n\t\r\v\f";

bool BuildTier(FamilyTier *tier, char* line); // Fills a FamilyTier struct with information
void FindChildren(FamilyTier familyTiers[MAX_TIERS], int currentTier, int numberOfTiers, int numberOfTabs);
int FindCorrespondingTier(FamilyTier familyTiers[MAX_TIERS], int currentTier, int numberOfTiers, bool *hasSpouse, char* childName);

int main(int argc, char *argv[])
{
    // Variables
    FamilyTier familyTiers[MAX_TIERS];
    char* filename;
    FILE* file;
    char lineBuffer[MAX_COLUMNS];
    int tierCounter = -1;
    int numTiers;

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

    // Find the children
    numTiers = tierCounter;
    FindChildren(familyTiers, FIRST_TIER, numTiers, FIRST_INDENT);

    return 0;
}

bool BuildTier(FamilyTier *tier, char* line)
{
    // Variables
    char* parent_1;
    char* parent_2;
    int childCounter = 0;
    char* childName;

    // Read in parent names from the file
    parent_1 = strtok(line, delimiters);
    if(parent_1 == NULL) return false;
    STRCPY(tier->parent_1, parent_1);

    parent_2 = strtok(NULL, delimiters);
    if(parent_2 == NULL) return false;
    STRCPY(tier->parent_2, parent_2);

    // The rest of the words in the line are children
    childName = strtok(NULL, delimiters);
    while(childName != NULL)
    {
        // Add the child name to the current family tier
        STRCPY(tier->children[childCounter], childName);

        // Get the next child
        childName = strtok(NULL, delimiters);
        childCounter++;
    }

    // Store the number of children we've counted
    tier->numberOfChildren = childCounter;

    return true;
}

void FindChildren(FamilyTier familyTiers[MAX_TIERS], int currentTier, int numberOfTiers, int numberOfTabs)
{
    // Variables
    FamilyTier* tier = &familyTiers[currentTier];
    int count;
    int tierCount;
    char* childName;
    bool hasSpouse;
    pid_t pid;

    // Print married name
    PrintTabs(numberOfTabs);
    printf("%s(%i)-%s\n", tier->parent_1, getpid(), tier->parent_2);

    numberOfTabs++;

    pid = fork();

    if(pid == 0)
    {
        // Iterate through all children
        for(count = 0; count < tier->numberOfChildren; count++)
        {
            childName = tier->children[count];
            hasSpouse = false;

            // Iterate through the rest of the tiers
            tierCount = FindCorrespondingTier(familyTiers, currentTier, numberOfTiers, &hasSpouse, childName);

            if(hasSpouse)
            {
                // Run the function again with the index of the struct containing child info
                FindChildren(familyTiers, tierCount, numberOfTiers, numberOfTabs);
            }
            else
            {
                PrintTabs(numberOfTabs);
                printf("%s(%i)\n", childName, getpid());
            }
        }

       // Exit the program after iterating through all the children
       exit(0);
    }
    else if(pid > 0)
    {
        // Wait for all children to die
        waitpid(pid, NULL, 0);
    }
}

int FindCorrespondingTier(FamilyTier familyTiers[MAX_TIERS], int currentTier, int numberOfTiers, bool *hasSpouse, char* childName)
{
    int tierCount;
    FamilyTier* tierPtr;

    // Iterate through the rest of the lines in the file
    // They have been stored as structs by this point, one struct per line
    for(tierCount = currentTier; tierCount < numberOfTiers; tierCount++)
    {
        // Create a pointer for convenience and readability
        tierPtr = &familyTiers[tierCount];

        // If the parent_1 or parent_2 matches the child's name
        if((strcmp(childName, tierPtr->parent_1) == 0) ||
                (strcmp(childName, tierPtr->parent_2) == 0))
        {
            // Then he/she is married
            *hasSpouse = true;
            break;
        }
    }

    // Return the index of the line we found the child in
    return tierCount;
}
