#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <list>
#include <string>
#include <memory>
#include "Operation.h"

enum States
{
    NEW, READY, RUNNING, WAITING, EXIT
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
        bool completed();
        int operations_left();
        int time_left();
        int processID() const;
        int duration() const;

        void add_operation( const std::string &operationString,
                           int hardwareCycleTime );

        std::list<Operation> operations();

        void step(); // Steps current operation

        bool operator>(const Program &other) const;

    private:

        std::list<Operation> operations_;

        struct
        {
            int state;
            int processID;
            int remainingTime;
            bool completed;
        } processControlBlock;
};

#endif


