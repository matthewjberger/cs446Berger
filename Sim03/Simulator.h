#ifndef SIMULATOR_H_
#define SIMULATOR_H_

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
#include <iomanip>
#include <algorithm>
#include <list>

#include "Program.h"

#define VERSION "3.0"
#define TRAILING_PRECISION 6
#define LEADING_PRECISION (3 + TRAILING_PRECISION + 1) // +1 for the decimal

class Simulator
{
    public:

        Simulator(const std::string &configFile);
        ~Simulator();

        void run();

    private:

       enum LoggingMode
        {
            LOG_TO_MONITOR,
            LOG_TO_FILE,
            LOG_TO_BOTH
        };

        enum SchedulingCode
        {
            FIFO_P,
            RR
        };

        struct
        {
            std::string version;
            std::string filePath;

            int processorCycleTime;
            int monitorDisplayTime;
            int hardDriveCycleTime;
            int printerCycleTime;
            int keyboardCycleTime;

            int quantumCycleTime;

            LoggingMode loggingMode;
            std::string logFilePath;

            SchedulingCode schedulingCode;
        } configurationData;

        /***** Member Functions *****/

        // Handles Program operations
        void handleOperation( const Operation& operation );

        // Handles IO with a unique thread
        void handleIO(const Operation& operation );

        // Outputs to monitor, log, or both depending on current loggingMode
        void display( const std::string &output );

        // Makes the current thread sleep for an amount of time using chrono
        void wait( int milliseconds ) const;

        // Returns the amount of time that has passed
        std::chrono::duration<double> secondsPassed();

        // A queue for SRTFN scheduling
        std::queue<Program> programsSRTFN_;

        // Scheduling Handlers
        void executeFIFOP();
        void executeRR();

        bool parseMetaData();
        bool parseConfigurationFile(const std::string &configFile );
        bool setLoggingMode( const std::string &loggingMode );
        bool setSchedulingCode( const std::string &schedulingCode );
        bool checkVersion() const;
        void displayErrorMessage(const std::string &message) const;
        void displayRemoveProcessText();
        void displayLoadProcessText();

        /***** Member Variables *****/
        std::list<Program> programs_;

        std::stringstream logStream_;

        std::chrono::time_point<std::chrono::high_resolution_clock>
            initialTime_, currentTime_;

        int processCount_;
        std::string processText;
};

#endif
