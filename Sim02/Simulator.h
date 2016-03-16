#ifndef __SIMULATOR_H
#define __SIMULATOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <queue>
#include <chrono>
#include <thread>
#include <ctime>
#include <pthread.h>
#include <unistd.h>
#include <limits>
#include <stdexcept>

#include "Program.h"

enum LoggingMode
{
    LOG_TO_MONITOR = 0x01,
    LOG_TO_FILE    = 0x02,
    LOG_TO_BOTH    = 0x04
};

enum SchedulingCode
{
    FIFO   = 0,
    SJF    = 1,
    SRTF_N = 2
};

class Simulator
{
    public:
        Simulator(const std::string configFile);
        ~Simulator();

        void Run();

    private:

        // Configuration Data
        struct ConfigurationData
        {
            // File information
            std::string version;
            std::string filePath;

            // Cycle times
            int processorCycleTime;
            int monitorDisplayTime;
            int hardDriveCycleTime;
            int printerCycleTime;
            int keyboardCycleTime;

            // Logging
            LoggingMode loggingMode;
            std::string logFilePath;

            // Scheduling Code
            SchedulingCode schedulingCode;
        };

        ConfigurationData configurationData;

        // Program Actions
        struct Operation
        {
            char componentLetter;    // S, A, P, I, O
            std::string description; // start, end, run, hard drive, keyboard, printer, monitor
            int cycleTime;           // Number of cycles the operation takes
        };

        // Programs
        struct Program
        {
            int pid;
            std::queue<Operation> operations;
        };

        // Handles an operation
        void Handle_Operation( const Operation* operation );

        // Handles IO with a unique thread
        void Handle_IO( const Operation* operation );

        // Outputs time and action
        void Display( std::string output );

        // Makes the current thread sleep for an amount of time using chrono
        void Wait( int milliseconds );

        // Returns the amount of time that has passed
        std::chrono::duration<double> secondsPassed();

        // Log file handle
        std::ofstream logFile;

        // Timing
        std::chrono::time_point<std::chrono::high_resolution_clock> initialTime, currentTime;

        // Fills out the configuration data struct
        void ParseConfigurationFile( std::string configFile );

};

#endif
