#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "Utilities.h"

// Meta-Data operations
struct Operation
{
    // S - Operating 'S'ystem (start and end),
    // A - Program 'A'pplication (start and end),
    // P- 'P'rocess (run),
    // I - 'I'nput,
    // or O - 'O'utput
    char componentLetter;

    std::string description; // end, hard drive, keyboard, printer, monitor, run, start
    int cycleTime; // Number of cycles the operation takes
};

class Program
{
    public:
        Program(const std::string metaDataFile);
        ~Program();

        std::vector<Operation> operations;

    private:

        void ParseMetaData(const std::string file_path)

};

#endif
