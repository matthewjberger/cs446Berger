#ifndef OPERATION_H_
#define OPERATION_H_

#include <string>

struct OperationParameters
{
    // S, A, P, I, O
    char id_;

    // start, end, run, hard drive, keyboard, printer, monitor
    std::string description_;

    // How long the operation needs to complete
    int duration_;

    // Number of remaining cycles
    int cyclesLeft_;

    // Cycle duration
    int cycleTime_;
};

class Operation
{
    public:

        Operation( const OperationParameters& parameters );

        bool completed() const;
        void step();

        OperationParameters parameters();

    private:

        OperationParameters parameters_;

};
#endif
