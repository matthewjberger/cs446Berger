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

class Program
{
    public:

        Program();
        ~Program();

        void run();
        void exit();
        void prepare();
        void addOperation( const std::string &operationString,
                           int hardwareCycleTime );
        void clearOperations();
        int processID() const;
        int duration() const;
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


