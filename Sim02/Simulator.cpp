#include "Simulator.h"
using namespace std;

Simulator::Simulator(const string &configFile)
{
    const string ERROR_MESSAGE = "Could not create the simulator";

    // Load configuration data
    if(!parseConfigurationFile(configFile))
    {
        throw new std::runtime_error(ERROR_MESSAGE);
    }

    // Load metadata
    if(!parseMetaData())
    {
        throw new std::runtime_error(ERROR_MESSAGE);
    }

    // Set how precise the times should be when displayed
    cout.precision(6);

    // Open the log file if necessary
    if(configurationData.loggingMode | LOG_TO_BOTH | LOG_TO_FILE)
    {
        logFile_.open(configurationData.logFilePath);
    }
}

Simulator::~Simulator()
{
    if(logFile_.is_open())
    {
        logFile_.close();
    }
}

void Simulator::display(const string &output)
{
    const auto timePassed = secondsPassed().count();

    if( configurationData.loggingMode |  LOG_TO_BOTH | LOG_TO_MONITOR )
    {
        cout << fixed << timePassed << " - " << output << endl;
    }

    if( configurationData.loggingMode | LOG_TO_BOTH | LOG_TO_FILE )
    {
        logFile_ << fixed << timePassed << " - " << output << endl;
    }
}

chrono::duration<double> Simulator::secondsPassed()
{
    // Get the time in seconds passed since the simulator was started
    currentTime_ = chrono::high_resolution_clock::now();
    return (currentTime_ - initialTime_);
}

void Simulator::wait(int milliseconds) const
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

bool Simulator::parseConfigurationFile(const std::string &configFile)
{
    // Variables
    string lineBuffer;
    const string INVALID_FILE_MESSAGE = "Invalid configuration file specified.";
    const long int LIMIT = numeric_limits<streamsize>::max();
    const bool DONT_IGNORE = false;
    ifstream inFile;

    // Define an error lambda
    auto HandleError = [this, INVALID_FILE_MESSAGE, &inFile]()
    {
        displayErrorMessage(INVALID_FILE_MESSAGE);

        // Make sure the file is closed properly
        if(inFile.is_open())
        {
            inFile.close();
        }

        return false;
    };

    // Open the file for reading
    inFile.open(configFile.c_str());

    // If file failed to open
    if(inFile.fail())
    {
        return HandleError();
    }

    // Check first line
    getline(inFile, lineBuffer);

    if(lineBuffer != "Start Simulator Configuration File")
    {
        return HandleError();
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

    return true;
}

bool Simulator::parseMetaData()
{
    return true;
}

void Simulator::run()
{
    // Execute all operations in the queue
    switch(configurationData.schedulingCode)
    {
        case FCFS:
        {
            executeFCFS();
            break;
        }

        case SJF:
        {
            executeSJF();
            break;
        }

        case SRTF_N:
        {
            executeSRTFN();
            break;
        }
    }
}

void Simulator::handleIO(const Operation& operation)
{
    string type;

    // Handle specific types of IO
    if(operation.description() == "hard drive")
    {
        if(operation.id() == 'I')
        {
            type = "input";
        }
        else if(operation.id() == 'O')
        {
            type = "output";
        }

        // display output
        display("Process 1: start hard drive " + type);
        wait(operation.cycleTime() * configurationData.hardDriveCycleTime);
        display("Process 1: end hard drive " + type);
    }
    else if(operation.description() == "keyboard")
    {
        display("Process 1: start keyboard input");
        wait(operation.cycleTime() * configurationData.keyboardCycleTime);
        display("Process 1: end keyboard input");
    }
    else if(operation.description() == "printer")
    {
        display("Process 1: start printer output");
        wait(operation.cycleTime() * configurationData.printerCycleTime);
        display("Process 1: end printer output");
    }
    else if(operation.description() == "monitor")
    {
        display("Process 1: start monitor output");
        wait(operation.cycleTime() * configurationData.monitorDisplayTime);
        display("Process 1: end monitor output");
    }
}

void Simulator::handleOperation(const Operation& operation)
{
    switch(operation.id())
    {
        // Simulator
        case 'S':
        {
            if(operation.description() == "start")
            {
                initialTime_ = chrono::high_resolution_clock::now();
                display("Simulator program starting");
            }
            else if(operation.description() == "end")
            {
                display("Simulator program ending");
            }
            break;
        }

        // Application
        case 'A':
        {
            if(operation.description() == "start")
            {
                display("OS: preparing process 1");
                display("OS: starting process 1");
            }
            else if(operation.description() == "end")
            {
                display("OS: removing process 1");
            }
            break;
        }

        // Processing
        case 'P':
        {
            display("Process 1: start processing action");
            wait(operation.cycleTime() * configurationData.processorCycleTime);
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

bool Simulator::setLoggingMode( const string &loggingMode )
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
        displayErrorMessage(errorMessage);
        return false;
    }

    return true;
}

bool Simulator::setSchedulingCode(const string &schedulingCode)
{
    const string ERROR_MESSAGE =
        "Invalid scheduling code specified. Could not set scheduling code";

    if(schedulingCode == "FCFS")
    {
        configurationData.schedulingCode = FCFS;
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
        displayErrorMessage(ERROR_MESSAGE);
        return false;
    }

    return true;
}

bool Simulator::checkVersion() const
{
    const string ERROR_MESSAGE =
        "Invalid version specified in configuration file";

    if(configurationData.version != VERSION)
    {
        displayErrorMessage(ERROR_MESSAGE);
        return false;
    }

    return true;
}

void Simulator::displayErrorMessage(const string &message) const
{
    cerr << "Error: " << message << endl;
}

