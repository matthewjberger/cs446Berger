#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <queue>
#include <string>

enum States
{
    NEW       = 0,
    READY     = 1,
    WAITING   = 2,
    RUNNING   = 3,
    SUSPENDED = 4
};

struct Operation
{
    // S, A, P, I, O
    char id;

    // start, end, run, hard drive, keyboard, printer, monitor
    std::string description;

    // Number of cycles the operation takes
    int cycleTime;
};

class Program
{
    public:

        Program();
        ~Program();

        void UpdateTotalTime(int time);
        void addOperation(const std::string &operationString);
        int GetPID();

    private:

        std::queue<Operation> operations;

        struct
        {
            int state;
            int pid;
            int totalTime; // Sum of time taken by operations
        } processControlBlock;
};

#endif


