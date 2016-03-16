#include "Simulator.h"
using namespace std;

Simulator::Simulator(const string &configFile)
{
    // Load configuration data
    parseConfigurationFile(configFile);

    // TODO: Load program data

    // Set how precise the times should be when displayed
    cout.precision(6);

    // Open the log file if necessary
    if(configurationData.loggingMode | LOG_TO_BOTH | LOG_TO_FILE)
    {
        logFile.open(configurationData.logFilePath);
    }
}

Simulator::~Simulator()
{
    if(logFile.is_open())
    {
        logFile.close();
    }
}

void Simulator::display(const string &output)
{
    auto timePassed = secondsPassed().count();

    if(configurationData.loggingMode |  LOG_TO_BOTH | LOG_TO_MONITOR)
    {
        cout << fixed << timePassed << " - " << output << endl;
    }

    if(configurationData.loggingMode | LOG_TO_BOTH | LOG_TO_FILE)
    {
        logFile << fixed << timePassed << " - " << output << endl;
    }
}

chrono::duration<double> Simulator::secondsPassed()
{
    // Get the time in seconds passed since the simulator was started
    currentTime = chrono::high_resolution_clock::now();
    return (currentTime - initialTime);
}

void Simulator::wait(int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void Simulator::parseConfigurationFile(const std::string &configFile)
{
    // Variables
    string lineBuffer;
    const string INVALID_FILE_MESSAGE = "Invalid configuration file specified.";
    const long int LIMIT = numeric_limits<streamsize>::max();
    const bool DONT_IGNORE = false;
    ifstream inFile;

    try
    {
        // Open the file for reading
        inFile.open(configFile.c_str());

        // If file failed to open
        if(inFile.fail())
        {
            throw new exception;
        }

        // Check first line
        getline(inFile, lineBuffer);
        if(lineBuffer != "Start Simulator Configuration File")
        {
            throw new exception;
        }

        // Declare a lambda to handle repeatedly getting input
        auto GetNextToken = [&inFile, LIMIT](const bool ignoreActive = true)
        {
            string nextToken;

            if(ignoreActive)
            {
                inFile.ignore(LIMIT, ':');
            }

            inFile >> nextToken;

            return nextToken;
        };

        // File Information
        configurationData.version = GetNextToken();
        checkVersion();

        // Metadata path
        configurationData.filePath = GetNextToken();

        // Scheduling code
        setSchedulingCode(GetNextToken());

        // Cycle times
        configurationData.processorCycleTime = stoi(GetNextToken());
        configurationData.monitorDisplayTime = stoi(GetNextToken());
        configurationData.hardDriveCycleTime = stoi(GetNextToken());
        configurationData.printerCycleTime   = stoi(GetNextToken());
        configurationData.keyboardCycleTime  = stoi(GetNextToken());

        // Get logging mode
        GetNextToken(); // 'Log'
        GetNextToken(DONT_IGNORE); // 'to'
        setLoggingMode(GetNextToken(DONT_IGNORE)); // 'Both/File/Monitor'

        // Log File Path
        configurationData.logFilePath = GetNextToken();

        if(inFile.is_open())
        {
            inFile.close();
        }
    }

    catch( ... )
    {
        if(inFile.is_open())
        {
            inFile.close();
        }

        handleError(INVALID_FILE_MESSAGE);
    }
}

void Simulator::parseMetaDa


void Simulator::run()
{
    // Execute all operations in the queue
/*
    while(!program->operations.empty())
    {
        Handle_Operation(&program->operations.front());

        // Remove the operations after they are handled
        program->operations.pop();
    }
*/
}

void Simulator::handleIO(const Operation* operation)
{
    string type;

    // Handle specific types of IO
    if(operation->description == "hard drive")
    {
        if(operation->id == 'I')
        {
            type = "input";
        }
        else if(operation->id == 'O')
        {
            type = "output";
        }

        // display output
        display("Process 1: start hard drive " + type);
        wait(operation->cycleTime * configurationData.hardDriveCycleTime);
        display("Process 1: end hard drive " + type);
    }
    else if(operation->description == "keyboard")
    {
        display("Process 1: start keyboard input");
        wait(operation->cycleTime * configurationData.keyboardCycleTime);
        display("Process 1: end keyboard input");
    }
    else if(operation->description == "printer")
    {
        display("Process 1: start printer output");
        wait(operation->cycleTime * configurationData.printerCycleTime);
        display("Process 1: end printer output");
    }
    else if(operation->description == "monitor")
    {
        display("Process 1: start monitor output");
        wait(operation->cycleTime * configurationData.monitordisplayTime);
        display("Process 1: end monitor output");
    }
}

void Simulator::handleOperation(const Operation* operation)
{
    switch(operation->id)
    {
        // Simulator
        case 'S':
        {
             if(operation->description == "start")
             {
                 initialTime = chrono::high_resolution_clock::now();
                 display("Simulator program starting");
             }
             else if(operation->description == "end")
             {
                 display("Simulator program ending");
             }
             break;
        }

        // Application
        case 'A':
        {
            if(operation->description == "start")
            {
                display("OS: preparing process 1");
                display("OS: starting process 1");
            }
            else if(operation->description == "end")
            {
                display("OS: removing process 1");
            }
            break;
        }

        // Processing
        case 'P':
        {
            display("Process 1: start processing action");
            wait(operation->cycleTime * configurationData.processorCycleTime);
            display("Process 1: start processing action");
            break;
        }

        // I/O
        case 'I':
        case 'O':
        {
            // Create a new thread for the IO operation
            thread IO_Thread(
                    [this, operation]()
                    {
                        // Run this function when the thread executes
                        handleIO(operation);
                    });

            // Join it to wait for the thread to complete
            if(IO_Thread.joinable())
            {
                IO_Thread.join();
            }

            break;
        }
    }
}

void Simulator::setLoggingMode( const string &loggingMode )
{
    string errorMessage =
        "Invalid loggingMode specified. Could not set logging mode.";

    if(loggingMode == "Both")
    {
        configurationData.loggingMode = LOG_TO_BOTH;
    }
    else if(loggingMode == "File")
    {
        configurationData.loggingMode = LOG_TO_FILE;
    }
    else if(loggingMode == "Monitor")
    {
        configurationData.loggingMode = LOG_TO_MONITOR;
    }
    else
    {
        handleError(errorMessage);
    }
}

void Simulator::setSchedulingCode(const string &schedulingCode)
{
    const string ERROR_MESSAGE =
        "Invalid scheduling code specified. Could not set scheduling code";

    if(schedulingCode == "FIFO")
    {
        configurationData.schedulingCode = FIFO;
    }
    else if(schedulingCode == "SJF")
    {
        configurationData.schedulingCode = SJF;
    }
    else if(schedulingCode == "SRTF_N")
    {
        configurationData.schedulingCode = SRTF_N;
    }
    else
    {
        handleError(ERROR_MESSAGE);
    }
}

void Simulator::checkVersion()
{
    const string ERROR_MESSAGE =
        "Invalid version specified in configuration file";

    if(configurationData.version != VERSION)
    {
        handleError(ERROR_MESSAGE);
    }
}

void Simulator::handleError(const string &message)
{
    cerr << "Error: " << message << endl;
    throw new exception;
}

