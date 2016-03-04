#include "Configuration.h"

using namespace std;

ConfigurationData::ConfigurationData(std::string configFile)
{
    loadedSuccessfully = false;
    ParseConfigFile(configFile);
}

ConfigurationData::~ConfigurationData(){}

int ConfigurationData::GetHardDriveCycleTime()
{
    return hardDriveCycleTime;
}

int ConfigurationData::GetKeyboardCycleTime()
{
    return keyboardCycleTime;
}

int ConfigurationData::GetMonitorDisplayTime()
{
    return monitorDisplayTime;
}

int ConfigurationData::GetPrinterCycleTime()
{
    return printerCycleTime;
}

int ConfigurationData::GetProcessorCycleTime()
{
    return processorCycleTime;
}

LoggingMode ConfigurationData::GetLoggingMode()
{
    return loggingMode;
}

std::string ConfigurationData::GetLogFilePath()
{
    return logFilePath;
}

std::string ConfigurationData::GetFilePath()
{
    return filePath;
}

std::string ConfigurationData::GetVersion()
{
    return version;
}

bool ContainsString(string source, string searchTerm)
{
    if(source.find(searchTerm) != std::string::npos)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Get everything in string
// after first occurence of ': '
string GetDataFromLine(string input)
{
    if(!ContainsString(input, ":"))
    {
        return "";
    }

    return input.substr(input.find(':') + 2);
}

void ConfigurationData::ParseConfigFile(std::string configFile)
{
    string lineBuffer;

    ifstream inFile(configFile.c_str());

    while(std::getline(inFile, lineBuffer))
    {
        if(ContainsString(lineBuffer, "Start Simulator") || ContainsString(lineBuffer, "End Simulator"))
        {
            // Skip the first and last line
        }
        else if(ContainsString(lineBuffer, "Version"))
        {
            version = GetDataFromLine(lineBuffer);
        }
        else if(ContainsString(lineBuffer, "Processor"))
        {
            processorCycleTime = atoi(GetDataFromLine(lineBuffer).c_str());
        }
        else if(ContainsString(lineBuffer, "Monitor"))
        {
            monitorDisplayTime = atoi(GetDataFromLine(lineBuffer).c_str());
        }
        else if(ContainsString(lineBuffer, "Hard drive"))
        {
            hardDriveCycleTime = atoi(GetDataFromLine(lineBuffer).c_str());
        }
        else if(ContainsString(lineBuffer, "Printer"))
        {
            printerCycleTime = atoi(GetDataFromLine(lineBuffer).c_str());
        }
        else if(ContainsString(lineBuffer, "Keyboard"))
        {
            keyboardCycleTime = atoi(GetDataFromLine(lineBuffer).c_str());
        }
        else if(ContainsString(lineBuffer, "Log:"))
        {
            string logType = GetDataFromLine(lineBuffer);
            if(ContainsString(logType, "Log to Both"))
            {
                loggingMode = LOG_TO_BOTH;
            }
            else if(ContainsString(logType, "Log to File"))
            {
                loggingMode = LOG_TO_FILE;
            }
            else if(ContainsString(logType, "Log to Monitor"))
            {
                loggingMode = LOG_TO_MONITOR;
            }
            else
            {
                loadedSuccessfully = false;
            }
        }
        else if(ContainsString(lineBuffer, "Log File Path:"))
        {
            logFilePath = GetDataFromLine(lineBuffer);
        }
        else if(ContainsString(lineBuffer, "File Path:"))
        {
            filePath = GetDataFromLine(lineBuffer);
        }
    }

    inFile.close();
    loadedSuccessfully = true;
}

bool ConfigurationData::LoadSuccessfully()
{
    return loadedSuccessfully;
}

void ConfigurationData::PrintData()
{
    if(!loadedSuccessfully)
    {
        cout << "Data was not loaded successfully!" << endl;
        return;
    }

    // Determine logging mode
    string logMode;
    if(loggingMode == LOG_TO_BOTH)
    {
        logMode = "Log to Both";
    }
    else if(loggingMode == LOG_TO_FILE)
    {
        logMode = "Log to File";
    }
    else if(loggingMode == LOG_TO_MONITOR)
    {
        logMode = "Log to Monitor";
    }

    // Display configuration data
    cout << "Simulation Configuration Data: " << endl;
    cout << "Version: " << version << endl;
    cout << "File path: " << filePath << endl;
    cout << "Processor cycle time: " <<  processorCycleTime << endl;
    cout << "Monitor display time: " << monitorDisplayTime << endl;
    cout << "Hard drive cycle time: " << hardDriveCycleTime << endl;
    cout << "Printer cycle time: " << printerCycleTime << endl;
    cout << "Keyboard cycle time: " << keyboardCycleTime << endl;
    cout << "Logging mode: " << logMode << endl;
    cout << "Log file path: " << logFilePath << endl;
}

