#ifndef OPERATION_H_
#define OPERATION_H_

#include <string>

struct OperationParameters
{
    // S, A, P, I, O
    char id;

    // start, end, run, hard drive, keyboard, printer, monitor
    std::string description;

    // How long the operation needs to complete
    int duration;

    // Number of remaining cycles
    int cyclesLeft;

    // Cycle duration
    int cycleTime;
};

class Operation
{
    public:

        Operation( const OperationParameters& parameters );

        bool completed() const;
        void step();

        OperationParameters parameters() const;

    private:

        OperationParameters parameters_;

};
#endif
