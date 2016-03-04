#ifndef __SIMULATOR_H
#define __SIMULATOR_H

#include "Utilities.h"
#include "ConfigurationData.h"
#include "Program.h"

enum IO
{
    INPUT  = 0,
    OUTPUT = 1,
};

class Simulator
{
    public:
        Simulator(const std::string configFile);
        ~Simulator();

        void Run();

    private:

        // Handles an operation
        void Handle_Operation(const Operation* operation);

        // Handles IO with a unique thread
        void Handle_IO(const Operation* operation);

        // Outputs time and action
        void Display(std::string output);

        // Makes the current thread sleep for an amount of time using chrono
        void Wait(int milliseconds);

        // Loads and stores configuration data
        ConfigurationData *configurationData;

        // The single program for Sim01
        Program *program;

        // Returns the amount of time that has passed
        std::chrono::duration<double> secondsPassed();

        // Log file handle
        std::ofstream logFile;

        // Timing
        std::chrono::time_point<std::chrono::high_resolution_clock> initialTime, currentTime;
};

#endif
