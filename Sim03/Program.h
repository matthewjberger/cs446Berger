#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <queue>
#include <string>
#include <memory>
#include <list>
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

        void prepare();
        void run();
        void suspend();
        void exit();
        void addOperation( const std::string &operationString,
                           int hardwareCycleTime );
        void clearOperations();
        int processID() const;
        int duration() const;
        std::list<Operation> operations();

        Operation nextOperation();

        bool operator>(const Program &other) const;

    private:

        std::list<Operation>::const_iterator currentOperation;

        std::list<Operation> operations_;

        struct
        {
            int state;
            int processID;
            int remainingTime;
        } processControlBlock;
};

#endif


