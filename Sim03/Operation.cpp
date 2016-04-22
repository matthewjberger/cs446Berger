#include "Operation.h"

Operation::Operation( const OperationParameters& parameters )
{
    parameters_ = parameters;
}

bool Operation::completed() const
{
    return (parameters_.cyclesLeft_ == 0);
}

void Operation::step()
{
    parameters_.duration_ -= parameters_.cycleTime_;
    parameters_.cyclesLeft_--;
}

OperationParameters Operation::parameters()
{
    return parameters_;
}
