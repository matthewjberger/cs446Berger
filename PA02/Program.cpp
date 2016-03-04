#include "Program.h"
using namespace std;

Program::Program(const string metaDataFile)
{
    ParseMetaData(metaDataFile);
}

Program::~Program()
{
}

// This will load the queue up with operations
// as described by the meta-data file.
void Program::ParseMetaData(const string filePath)
{
    string lineBuffer;
    Operation operation;

    // Open the file for reading
    ifstream inFile(filePath.c_str());
    if(inFile.fail())
    {
        cerr << "Error! Failed to open Meta-Data file." << endl;
        return;
    }

    // Check first line
    getline(inFile, lineBuffer);
    if(lineBuffer != "Start Program Meta-Data Code:")
    {
        cerr << "Error! Invalid Meta-Data file." << endl;
        inFile.close();
        return;
    }


    // Get each token delimited by semicolons
    while(lineBuffer != "S(end)0")
    {
        // Ignore whitespace
        inFile >> std::ws;

        // Token format is S(start)0
        getline(inFile, lineBuffer, ';');

        // Create the operation
        CreateOperationFromMetaData(operation, lineBuffer);

        // Load the operation into the queue
        operations.push(operation);

        // At the end of the first program
        if(lineBuffer == "A(end)0")
        {
            // Ignore whitespace
            inFile >> std::ws;

            // Get the final operation S(end)0
            getline(inFile, lineBuffer, '.');

            // Create the operation
            CreateOperationFromMetaData(operation, lineBuffer);

            // Load the operation into the queue
            operations.push(operation);
        }
    }

    // Close the file
    inFile.close();
}

void Program::CreateOperationFromMetaData(Operation &operation, string metaData)
{
    // Operation format is "S(start)0"

    // Get the type
    operation.componentLetter = metaData.front();

    // Get the operation description from inside the parentheses
    int firstParenLoc = metaData.find('(');
    int secondParenLoc = metaData.find(')');
    string description = metaData.substr(firstParenLoc + 1, secondParenLoc - firstParenLoc - 1);
    operation.description = description;

    // Get the duration
    string cycleTime = metaData.substr(secondParenLoc + 1);
    operation.cycleTime = stoi(cycleTime);
}

