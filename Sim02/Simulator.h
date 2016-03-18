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

#include "Program.h"

#define VERSION "2.0"

class Simulator
{
    public:

        Simulator(const std::string &configFile);
        ~Simulator();

        void run();

    private:

       enum LoggingMode
        {
            LOG_TO_MONITOR = 0x01,
            LOG_TO_FILE    = 0x02,
            LOG_TO_BOTH    = 0x04
        };

        enum SchedulingCode
        {
            FCFS   = 0,
            SJF    = 1,
            SRTF_N = 2
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

        // Scheduling Handlers
        void executeFCFS();
        void executeSJF();
        void executeSRTFN();

        bool parseMetaData();
        bool parseConfigurationFile(const std::string &configFile );
        bool setLoggingMode( const std::string &loggingMode );
        bool setSchedulingCode( const std::string &schedulingCode );
        bool checkVersion() const;
        void displayErrorMessage(const std::string &message) const;

        /***** Member Variables *****/
        std::queue<Program> programs_;

        std::ofstream logFile_;

        std::chrono::time_point<std::chrono::high_resolution_clock>
            initialTime_, currentTime_;
};

#endif
