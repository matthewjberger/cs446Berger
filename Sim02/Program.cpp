#include "Program.h"
using namespace std;

/****** Program ******/
Program::Program()
{
    processControlBlock.state      = NEW;
    processControlBlock.processID  = 0;
    processControlBlock.totalTime  = 0;
}

Program::~Program()
{
    while(!operations_.empty())
    {
        operations_.pop();
    }
}

void Program::addOperation(const std::string &operationString)
{
    // Operation format is "S(start)0"

    // Get the type
    char id = operationString.front();

    // Get the operation description from inside the parentheses
    int firstParenLoc  = operationString.find( '(' );
    int secondParenLoc = operationString.find( ')' );
    int beginning      = firstParenLoc + 1;
    int end            = secondParenLoc - firstParenLoc - 1;
    string description = operationString.substr( beginning, end );

    // Get the duration
    string cycleTimeString = operationString.substr( secondParenLoc + 1 );
    int cycleTime          = stoi(cycleTimeString);

    // Add the operation
    Operation operation(id, description, cycleTime);
    operations_.push(operation);
}

int Program::processID()
{
    return processControlBlock.processID;
}

void Program::updateTotalTime( int time )
{
    processControlBlock.totalTime = time;
}

/****** Operation ******/
Operation::Operation( const char id,
                      const std::string &description,
                      const int cycleTime )
{
    this->id_          = id;
    this->description_ = description;
    this->cycleTime_   = cycleTime;

}

char Operation::id() const
{
    return id_;
}

int Operation::cycleTime() const
{
    return cycleTime_;
}

std::string Operation::description() const
{
    return description_;
}


