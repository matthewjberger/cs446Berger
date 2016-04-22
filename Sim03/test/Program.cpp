#include "Program.h"
using namespace std;

Program::Program()
{
    processControlBlock.state      = NEW;
    processControlBlock.processID  = 0;
    processControlBlock.remainingTime  = 0;
    processControlBlock.completed = false;

    currentOperation_ = 0;
}

Program::~Program()
{
}

void Program::add_operation( const std::string &operationString,
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
    OperationParameters operationParameters;
    operationParameters.id = id;
    operationParameters.description = description;
    operationParameters.duration = duration;
    operationParameters.cycleTime = hardwareCycleTime;
    operationParameters.cyclesLeft = cycles;

    Operation operation( operationParameters );
    operations_.push_back( operation );

    // Update the program duration
    processControlBlock.remainingTime
        += operations_.front().parameters().duration;
}

void Program::clear_operations()
{
    while(!operations_.empty())
    {
        operations_.pop_back();
    }

    currentOperation_ = 0;
}

std::vector<Operation> Program::operations()
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

bool Program::operator>(const Program &other) const
{
    int firstTime = processControlBlock.remainingTime;
    int otherTime = other.processControlBlock.remainingTime;
    return (firstTime > otherTime);
}

void Program::suspend()
{
    processControlBlock.state = SUSPENDED;
}

int Program::operations_left() const
{
    return operations_.size() - currentOperation_;
}

Operation Program::step()
{
    Operation operation = operations_[currentOperation_];
    if(operation.completed())
    {
        printf("tesAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAt!\n");
        if(currentOperation_ < operations_.size() - 1)
        {
            currentOperation_++; // next operation in the list
        }
        else
        {
            processControlBlock.completed = true;
        }
    }

    operation.step();
    return operation;
}

PCB Program::process_control_block() const
{
    return processControlBlock;
}

void Program::assign_pid(int pid)
{
    processControlBlock.processID = pid;
}
