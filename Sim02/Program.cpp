#include "Program.h"
using namespace std;

Program::Program()
{
    processControlBlock.state = NEW;
    processControlBlock.pid = 0;
    processControlBlock.totalTime = 0;
}

Program::~Program()
{
}

void Program::addOperation(const std::string &operationString)
{
    // Operation format is "S(start)0"
    Operation operation;

    // Get the type
    operation.id = operationString.front();

    // Get the operation description from inside the parentheses
    int firstParenLoc = operationString.find('(');
    int secondParenLoc = operationString.find(')');
    string description = operationString.substr(firstParenLoc + 1, secondParenLoc - firstParenLoc - 1);
    operation.description = description;

    // Get the duration
    string cycleTime = operationString.substr(secondParenLoc + 1);
    operation.cycleTime = stoi(cycleTime);

    // Add the operation
    operations.push(operation);
}

int Program::GetPID()
{
    return processControlBlock.pid;
}
