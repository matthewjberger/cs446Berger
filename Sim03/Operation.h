#ifndef OPERATION_H_
#define OPERATION_H_

#include <string>

class Operation
{
    public:

        Operation( const char id,
                   const std::string &description,
                   const int duration  );

        char id() const;
        std::string description() const;
        int duration() const;

    private:

        // S, A, P, I, O
        char id_;

        // start, end, run, hard drive, keyboard, printer, monitor
        std::string description_;

        // The amount of time the operation takes to complete
        int duration_;
};
#endif
