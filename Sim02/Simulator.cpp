#include "Simulator.h"
using namespace std;

Simulator::Simulator(const string configFile)
{
    // Load configuration data
    ParseConfigurationFile(configFile);

    // Load program data
    program = new Program(configurationData.filePath);
    if(program == NULL)
    {
        cerr << "Failed to load the program for the simulator!" << endl;
        exit(0);
    }

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

    delete program;
    program = NULL;
}

void Simulator::Display(string output)
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

void Simulator::Handle_IO(const Operation* operation)
{
    // Handle specific types of IO
    if(operation->description == "hard drive")
    {
        string type;
        if(operation->componentLetter == 'I')
        {
            type = "input";
        }
        else if(operation->componentLetter == 'O')
        {
            type = "output";
        }

        // Display output
        Display("Process 1: start hard drive " + type);
        Wait(operation->cycleTime * configurationData.hardDriveCycleTime);
        Display("Process 1: end hard drive " + type);
    }
    else if(operation->description == "keyboard")
    {
        Display("Process 1: start keyboard input");
        Wait(operation->cycleTime * configurationData.keyboardCycleTime);
        Display("Process 1: end keyboard input");
    }
    else if(operation->description == "printer")
    {
        Display("Process 1: start printer output");
        Wait(operation->cycleTime * configurationData.printerCycleTime);
        Display("Process 1: end printer output");
    }
    else if(operation->description == "monitor")
    {
        Display("Process 1: start monitor output");
        Wait(operation->cycleTime * configurationData.monitorDisplayTime);
        Display("Process 1: end monitor output");
    }
}

void Simulator::Handle_Operation(const Operation* operation)
{
    switch(operation->componentLetter)
    {
        // Simulator
        case 'S':
            {
                if(operation->description == "start")
                {
                    initialTime = chrono::high_resolution_clock::now();
                    Display("Simulator program starting");
                }
                else if(operation->description == "end")
                {
                    Display("Simulator program ending");
                }
                break;
            }

            // Application
        case 'A':
            {
                if(operation->description == "start")
                {
                    Display("OS: preparing process 1");
                    Display("OS: starting process 1");
                }
                else if(operation->description == "end")
                {
                    Display("OS: removing process 1");
                }
                break;
            }

            // Processing
        case 'P':
            {
                Display("Process 1: start processing action");
                Wait(operation->cycleTime * configurationData.processorCycleTime);
                Display("Process 1: start processing action");
                break;
            }

            // I/O
        case 'I':
        case 'O':
            {
                // Create a new thread for the IO operation
                thread IO_Thread([this, operation]()
                        {
                        // Run this function when the thread executes
                        Handle_IO(operation);
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

void Simulator::Run()
{
    // Execute all operations in the queue
    while(!program->operations.empty())
    {
        Handle_Operation(&program->operations.front());

        // Remove the operations after they are handled
        program->operations.pop();
    }
}

chrono::duration<double> Simulator::secondsPassed()
{
    // Get the time in seconds passed since the simulator was started
    currentTime = chrono::high_resolution_clock::now();
    return (currentTime - initialTime);
}

void Simulator::Wait(int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void Simulator::ParseConfigurationFile(std::string configFile)
{
    // Variables
    string lineBuffer;
    string loggingMode;
    string invalid_file_message   = "Invalid configuration file specified.";
    long int limit = numeric_limits<streamsize>::max();
    const bool DONT_IGNORE = false;

    // Open the file for reading
    ifstream inFile(configFile.c_str());

    try
    {
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
        auto GetNextToken = [&inFile, invalid_file_message, limit](bool ignoreActive = true)
        {
            string nextToken;

            if(ignoreActive)
            {
                inFile.ignore(limit, ':');
            }

            inFile >> nextToken;

            return nextToken;
        };

        // File Information
        configurationData.version = GetNextToken();
        configurationData.filePath = GetNextToken();

        // Cycle times
        configurationData.processorCycleTime = stoi(GetNextToken());
        configurationData.monitorDisplayTime = stoi(GetNextToken());
        configurationData.hardDriveCycleTime = stoi(GetNextToken());
        configurationData.printerCycleTime   = stoi(GetNextToken());
        configurationData.keyboardCycleTime  = stoi(GetNextToken());

        // Get logging mode
        GetNextToken(); // 'Log'
        GetNextToken(DONT_IGNORE); // 'to'
        loggingMode = GetNextToken(DONT_IGNORE); // 'Both/File/Monitor'

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
            throw new exception;
        }

        configurationData.logFilePath = GetNextToken();

        if(inFile.is_open())
        {
            inFile.close();
        }
    }

    catch( ... )
    {
        cerr << "Error while reading file: " << invalid_file_message << endl;

        if(inFile.is_open())
        {
            inFile.close();
        }

        throw;
    }
}

