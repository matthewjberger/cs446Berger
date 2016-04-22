#include "Program.h"
using namespace std;

/****** Program ******/
Program::Program()
{
    processControlBlock.state      = NEW;
    processControlBlock.processID  = 0;
    processControlBlock.remainingTime  = 0;

    currentOperation = operations_.begin();
}

Program::~Program()
{
    clearOperations();
}

void Program::addOperation( const std::string &operationString,
                            int hardwareCycleTime )
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
    string cyclesString = operationString.substr( secondParenLoc + 1 );
    int cycles = stoi( cyclesString );
    int duration = ( cycles * hardwareCycleTime );

    // Add the operation
    Operation operation( id, description, duration );
    operations_.push_back( operation );

    // Update the program duration
    processControlBlock.remainingTime += operations_.front().duration();
}

int Program::processID() const
{
    return processControlBlock.processID;
}

void Program::clearOperations()
{
    if( !operations_.empty() )
    {
        operations_.clear();
        currentOperation = operations_.begin();
    }
}

std::list<Operation> Program::operations()
{
    return operations_;
}

void Program::run()
{
    processControlBlock.state = RUNNING;
}

void Program::exit()
{
    processControlBlock.state = EXIT;
}

void Program::prepare()
{
    processControlBlock.state = READY;
}

int Program::duration() const
{
    return processControlBlock.remainingTime;
}

Operation Program::nextOperation()
{
    processControlBlock.remainingTime -= currentOperation->duration();

    if(currentOperation != operations_.end())
    {
        currentOperation++;
    }

    return *currentOperation;
}

bool Program::operator>(const Program &other) const
{
    return (duration() > other.duration());
}

void Program::suspend()
{
    processControlBlock.state = WAITING;
}

