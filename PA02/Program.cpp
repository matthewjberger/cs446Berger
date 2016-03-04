#include "Program.h"
using namespace std;

Program::Program(const string metaDataFile)
{
    ParseMetaData(metaDataFile);
}

Program::~Program()
{
}

void Program::ParseMetaData(const string filePath)
{
    string lineBuffer;

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

    Operation operation;

    // Get each token delimited by semicolons
    while(lineBuffer != "A(end)0")
    {
        if(lineBuffer.back() == '.')
        {
            lineBuffer.pop_back();
        }

        // Ignore whitespace
        inFile >> std::ws;

        // Token format is S(start)0
        getline(inFile, lineBuffer, ';');

        // Get the type
        operation.componentLetter = lineBuffer.front();

        // Get the operation description
        int firstParenLoc = lineBuffer.find('(');
        int secondParenLoc = lineBuffer.find(')');
        string description = lineBuffer.substr(firstParenLoc + 1, secondParenLoc - firstParenLoc - 1);
        operation.description = description;

        // Get the duration
        string cycleTime = lineBuffer.substr(secondParenLoc + 1);
        operation.cycleTime = stoi(cycleTime);

        // Load the operation into the queue
        operations.push(operation);
    }

    // Ignore whitespace
    inFile >> std::ws;

    // Build the final S(end)0 operation
    getline(inFile, lineBuffer, '.');

    // Get the type
    operation.componentLetter = lineBuffer.front();

    // Get the operation description
    int firstParenLoc = lineBuffer.find('(');
    int secondParenLoc = lineBuffer.find(')');
    string description = lineBuffer.substr(firstParenLoc + 1, secondParenLoc - firstParenLoc - 1);
    operation.description = description;

    // Get the duration
    string cycleTime = lineBuffer.substr(secondParenLoc + 1);
    operation.cycleTime = stoi(cycleTime);

    // Load the operation into the queue
    operations.push(operation);

    inFile.close();
}

