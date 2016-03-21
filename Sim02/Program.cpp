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
    processControlBlock.totalTime += operations_.front().duration();
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
    return processControlBlock.totalTime;
}

/****** Operation ******/
Operation::Operation( const char id,
                      const std::string &description,
                      const int duration )
{
    this->id_          = id;
    this->description_ = description;
    this->duration_    = duration;

}

char Operation::id() const
{
    return id_;
}

int Operation::duration() const
{
    return duration_;
}

std::string Operation::description() const
{
    return description_;
}


