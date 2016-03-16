#ifndef __PROGRAM_H
#define __PROGRAM_H

#include <iostream>
#include <string>
#include <queue>
#include <fstream>

// Meta-Data operations
struct Operation
{
    char componentLetter;    // S, A, P, I, O
    std::string description; // start, end, run, hard drive, keyboard, printer, monitor
    int cycleTime;           // Number of cycles the operation takes
};


class Program
{
    public:
        Program( const std::string metaDataFile );
        ~Program();


        void AddOperationFromMetaData( std::string MetaData );
        std::queue<Operation> operations;

    private:
        void ParseMetaData( const std::string filePath );

};

#endif