#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <vector>
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

        void assign_pid(int pid);

        void add_operation( const std::string &operationString,
                           int hardwareCycleTime );

        void clear_operations();

        std::vector<Operation> operations();

        // Steps current operation, returns info about the operation
        Operation step();

        bool operator>(const Program &other) const;

    private:

        std::vector<Operation> operations_;
        unsigned int currentOperation_;

        PCB processControlBlock;
};

#endif


