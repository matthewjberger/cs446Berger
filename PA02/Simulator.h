#ifndef __SIMULATOR_H
#define __SIMULATOR_H

#include "Utilities.h"
#include "ConfigurationData.h"

class Simulator
{
    public:
        Simulator(const std::string configFile);
        ~Simulator();

        void Run();

    private:
        ConfigurationData *configurationData;

        // Runs through a program operation
        void Run_Operation();
};

#endif
