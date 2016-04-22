#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <list>
#include <string>
#include <memory>
#include "Operation.h"

enum States
{
    NEW, READY, RUNNING, SUSPENDED, EXIT
};

struct PCB
{
    int state;
    int processID;
    int remainingTime;
    bool completed;
};

class Program
{
    public:

        Program();
        ~Program();

        // State Manipulation
        void prepare();
        void run();
        void suspend();
        void exit();

        // Status
        int operations_left() const;
        PCB process_control_block() const;

        void add_operation( const std::string &operationString,
                           int hardwareCycleTime );

        void clear_operations();

        std::list<Operation> operations();

        Operation step(); // Steps current operation, returns info about the operation

        bool operator>(const Program &other) const;

    private:

        std::list<Operation> operations_;
        std::list<Operation>::iterator currentOperation_;

        PCB processControlBlock;
};

#endif


