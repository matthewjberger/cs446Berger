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

    // Ignore whitespace
    inFile >> std::ws;

    Operation operation;

    // Get each token delimited by semicolons
    while(lineBuffer != "A(end)0.")
    {
        if(lineBuffer.back() == '.')
        {
            lineBuffer.pop_back();
        }

        // Token format is S(start)0
        getline(inFile, lineBuffer, ';');

        // Get the type
        operation.componentLetter = lineBuffer[0];

        // Get the operation description
        int firstParenLoc = lineBuffer.find('(');
        int secondParenLoc = lineBuffer.find(')');
        string description = lineBuffer.substr(firstParenLoc, secondParenLoc - firstParenLoc);
        operation.description = description;

        // Get the duration
        string cycleTime = lineBuffer.substr(secondParenLoc + 1);
        operation.cycleTime = stoi(cycleTime);

        // Load the operation into the queue
        operations.push(operation);
    }

    // Build the final S(end)0 operation
    getline(inFile, lineBuffer, '.');

    // Get the type
    operation.componentLetter = lineBuffer[0];

    // Get the operation description
    int firstParenLoc = lineBuffer.find('(');
    int secondParenLoc = lineBuffer.find(')');
    string description = lineBuffer.substr(firstParenLoc, secondParenLoc - firstParenLoc);
    operation.description = description;

    // Get the duration
    string cycleTime = lineBuffer.substr(secondParenLoc + 1);
    operation.cycleTime = stoi(cycleTime);

    // Load the operation into the queue
    operations.push(operation);

    // Eat the final line
    getline(inFile,lineBuffer);

    // Check last line
    getline(inFile, lineBuffer);
    if(lineBuffer != "End Program Meta-Data Code.")
    {
        cerr << "Error! Invalid Meta-Data file." << endl;
        inFile.close();
        return;
    }

    inFile.close();
}

