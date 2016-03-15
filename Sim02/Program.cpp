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
    string invalid_file_message   = "Invalid Meta-Data file specified.";
    Operation operation;
    ifstream inFile;

    try
    {
        // Open the file for reading
        inFile.open(filePath.c_str());

        // If the file failed to open
        if(inFile.fail())
        {
            throw new exception;
        }

        // Check first line
        getline(inFile, lineBuffer);
        if(lineBuffer != "Start Program Meta-Data Code:")
        {
            throw new exception;
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

            if(inFile.eof())
            {
                throw new exception;
            }
        }

        // Close the file
        inFile.close();
    }

    catch( ... )
    {
        cerr << "Error while reading Meta-Data file: " << invalid_file_message << endl;

        if(inFile.is_open())
        {
            inFile.close();
        }

        throw;
    }
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

