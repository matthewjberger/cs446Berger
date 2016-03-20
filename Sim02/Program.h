#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <queue>
#include <string>
#include <memory>
#include <list>

enum States
{
    NEW       = 0,
    READY     = 1,
    WAITING   = 2,
    RUNNING   = 3,
    SUSPENDED = 4,
    EXIT      = 5
};

class Operation
{
    public:

        Operation( const char id,
                   const std::string &description,
                   const int cycleTime );

        char id() const;
        std::string description() const;
        int cycleTime() const;

    private:

        char id_; // S, A, P, I, O

        // start, end, run, hard drive, keyboard, printer, monitor
        std::string description_;

        int cycleTime_;// Number of cycles the operation takes
};

class Program
{
    public:

        Program();
        ~Program();

        void run();
        void exit();
        void updateTotalTime( int time );
        void addOperation( const std::string &operationString );
        void clearOperations();
        int processID();
        std::list<Operation> operations();

    private:

        std::list<Operation> operations_;

        struct
        {
            int state;
            int processID;
            int totalTime; // Sum of time taken by operations
        } processControlBlock;
};

#endif


