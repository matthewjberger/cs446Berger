#include "Operation.h"

Operation::Operation( const char id,
                      const std::string &description,
                      const int duration,
                      const int cycles )
{
    parameters_.id_          = id;
    parameters_.description_ = description;
    parameters_.duration_    = duration;
    parameters_.cyclesLeft_  = cycles;

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


