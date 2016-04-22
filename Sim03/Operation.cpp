#include "Operation.h"

Operation::Operation( const char id,
                      const std::string &description,
                      const int duration )
{
    id_          = id;
    description_ = description;
    duration_    = duration;

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


