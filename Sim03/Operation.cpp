#include "Operation.h"

Operation::Operation( const OperationParameters& parameters )
{
    parameters_ = parameters;
}

bool Operation::completed() const
{
    return (parameters_.cyclesLeft == 0);
}

void Operation::step()
{
    parameters_.duration -= parameters_.cycleTime;
    parameters_.cyclesLeft--;
}

OperationParameters Operation::parameters()
{
    return parameters_;
}
