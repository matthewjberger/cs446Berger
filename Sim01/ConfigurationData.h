#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "Utilities.h"

enum LoggingMode
{
    LOG_TO_MONITOR = 0x01,
    LOG_TO_FILE    = 0x02,
    LOG_TO_BOTH    = 0x04
};

class ConfigurationData
{
    public:
        ConfigurationData(std::string configFile);
        ~ConfigurationData();

        std::string GetVersion();
        std::string GetFilePath();
        int GetProcessorCycleTime();
        int GetMonitorDisplayTime();
        int GetHardDriveCycleTime();
        int GetPrinterCycleTime();
        int GetKeyboardCycleTime();

        // Logging
        LoggingMode GetLoggingMode();
        std::string GetLogFilePath();

        bool LoadedSuccessfully();

        void PrintData();

    private:

        void ParseConfigFile(std::string configFile);

        /* All timings are in msec */
        std::string version;
        std::string filePath;
        int processorCycleTime;
        int monitorDisplayTime;
        int hardDriveCycleTime;
        int printerCycleTime;
        int keyboardCycleTime;

        // Logging
        LoggingMode loggingMode;
        std::string logFilePath;

        bool loadedSuccessfully;
};

#endif
