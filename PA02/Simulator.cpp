#include "Simulator.h"
using namespace std;

Simulator::Simulator(const string configFile)
{
    configurationData = new ConfigurationData(configFile);
    if(configurationData == NULL || !configurationData->LoadedSuccessfully())
    {
        cerr << "Failed to load configuration data for the simulator!" << endl;
        exit(0);
    }

    program = new Program(configurationData->GetFilePath());
    if(program == NULL)
    {
        cerr << "Failed to load the program for the simulator!" << endl;
        exit(0);
    }

    // Set how precise the times should be when displayed
    cout.precision(6);

    if(configurationData->GetLoggingMode() == LOG_TO_BOTH || configurationData->GetLoggingMode() == LOG_TO_FILE)
    {
        logFile.open(configurationData->GetLogFilePath());
    }
}

Simulator::~Simulator()
{
    if(logFile.is_open())
    {
        logFile.close();
    }

    delete configurationData;
    delete program;

    configurationData = NULL;
    program = NULL;
}

void Simulator::Display(string output)
{
    chrono::duration<double> timePassed = secondsPassed();

    if(configurationData->GetLoggingMode() == LOG_TO_BOTH || configurationData->GetLoggingMode() == LOG_TO_MONITOR)
    {
        cout << fixed << timePassed.count() << " - " << output << endl;
    }

    if(configurationData->GetLoggingMode() == LOG_TO_BOTH || configurationData->GetLoggingMode() == LOG_TO_FILE)
    {
        logFile << fixed << timePassed.count() << " - " << output << endl;
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
        Wait(operation->cycleTime * configurationData->GetHardDriveCycleTime());
        Display("Process 1: end hard drive " + type);
    }
    else if(operation->description == "keyboard")
    {
        Display("Process 1: start keyboard input");
        Wait(operation->cycleTime * configurationData->GetKeyboardCycleTime());
        Display("Process 1: end keyboard input");
    }
    else if(operation->description == "printer")
    {
        Display("Process 1: start printer output");
        Wait(operation->cycleTime * configurationData->GetKeyboardCycleTime());
        Display("Process 1: end printer output");
    }
    else if(operation->description == "monitor")
    {
        Display("Process 1: start monitor output");
        Wait(operation->cycleTime * configurationData->GetMonitorDisplayTime());
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
            Wait(operation->cycleTime * configurationData->GetProcessorCycleTime());
            Display("Process 1: start processing action");
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
                    Handle_IO(operation);
                    });

            // Join it to wait for the thread to complete
            if(IO_Thread.joinable())
            {
                IO_Thread.join();
            }
        }
    }
}

void Simulator::Run()
{
    // Execute all operations in the queue
    while(!program->operations.empty())
    {
        Handle_Operation(&program->operations.front());
        program->operations.pop();
    }
}

chrono::duration<double> Simulator::secondsPassed()
{
    currentTime = chrono::high_resolution_clock::now();
    return (currentTime - initialTime);
}

void Simulator::Wait(int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

