#include "Simulator.h"
using namespace std;

Simulator::Simulator( const string &configFile )
{
    const string ERROR_MESSAGE = "Could not create the simulator";

    processCount_ = 0;
    processText = "Process 1: ";

    // Load configuration data
    if(!parseConfigurationFile( configFile ))
    {
        throw new std::runtime_error( ERROR_MESSAGE );
    }

    // Load metadata
    if(!parseMetaData())
    {
        throw new std::runtime_error( ERROR_MESSAGE );
    }

    // Set how precise the times should be when displayed
    cout.precision( TRAILING_PRECISION );
}

Simulator::~Simulator()
{
    // Write to the log if required
    if( configurationData.loggingMode == LOG_TO_BOTH ||
        configurationData.loggingMode == LOG_TO_FILE )
    {
        ofstream logFile_( configurationData.logFilePath );

        if(logFile_.good())
        {
            logFile_ << logStream_.str();
        }

        if(logFile_.is_open())
        {
            logFile_.close();
        }
    }
}

void Simulator::display( const string &output )
{
    const auto timePassed = secondsPassed().count();

    if( configurationData.loggingMode == LOG_TO_BOTH ||
        configurationData.loggingMode == LOG_TO_MONITOR )
    {
        cout << setw( LEADING_PRECISION ) << setfill( '0' )
             << fixed << timePassed << " - " << output << endl;

    }

    // If logging, write to a string stream and then log to file
    // at the end of the program
    if( configurationData.loggingMode == LOG_TO_BOTH ||
            configurationData.loggingMode == LOG_TO_FILE )
    {
        logStream_ << setw( LEADING_PRECISION ) << setfill( '0' )
            << fixed << timePassed << " - " << output << endl;
    }
}

chrono::duration<double> Simulator::secondsPassed()
{
    // Get the time in seconds passed since the simulator was started
    currentTime_ = chrono::high_resolution_clock::now();
    return ( currentTime_ - initialTime_ );
}

void Simulator::wait( int milliseconds ) const
{
    bool wait = true;

    auto start = std::chrono::system_clock::now();
    while(wait)
    {
        auto now = std::chrono::system_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        if ( elapsed.count() > milliseconds )
        {
            wait = false;
        }
    }
}

bool Simulator::parseConfigurationFile( const std::string &configFile )
{
    // Variables
    string lineBuffer;
    const string INVALID_FILE_MESSAGE = "Invalid configuration file specified.";
    const long int LIMIT = numeric_limits<streamsize>::max();
    const bool DONT_IGNORE = false;
    ifstream inFile;

    // Define an error lambda
    auto HandleError = [ this, INVALID_FILE_MESSAGE, &inFile ]()
    {
        displayErrorMessage( INVALID_FILE_MESSAGE );

        // Make sure the file is closed properly
        if( inFile.is_open() )
        {
            inFile.close();
        }

        return false;
    };

    // Open the file for reading
    inFile.open( configFile.c_str() );

    // If file failed to open
    if( inFile.fail() )
    {
        return HandleError();
    }

    // Check first line
    getline( inFile, lineBuffer );

    if( lineBuffer != "Start Simulator Configuration File" )
    {
        return HandleError();
    }

    // Declare a lambda to handle repeatedly getting input
    auto GetNextToken = [ &inFile, LIMIT ]( const bool ignoreActive = true )
    {
        string nextToken;

        if(ignoreActive)
        {
            inFile.ignore( LIMIT, ':' );
        }

        inFile >> nextToken;

        return nextToken;
    };

    // File Information
    configurationData.version = GetNextToken();
    if( !checkVersion() )
    {
        return false;
    }

    // Metadata path
    configurationData.filePath = GetNextToken();

    // Scheduling code
    setSchedulingCode( GetNextToken() );

    // Cycle times
    configurationData.quantum            = stoi( GetNextToken() );
    configurationData.processorCycleTime = stoi( GetNextToken() );
    configurationData.monitorDisplayTime = stoi( GetNextToken() );
    configurationData.hardDriveCycleTime = stoi( GetNextToken() );
    configurationData.printerCycleTime   = stoi( GetNextToken() );
    configurationData.keyboardCycleTime  = stoi( GetNextToken() );

    // Get logging mode
    GetNextToken(); // 'Log'
    GetNextToken( DONT_IGNORE ); // 'to'
    setLoggingMode( GetNextToken(DONT_IGNORE) ); // 'Both/File/Monitor'

    // Log File Path
    configurationData.logFilePath = GetNextToken();

    if( inFile.is_open() )
    {
        inFile.close();
    }

    return true;
}

bool Simulator::parseMetaData()
{
    string lineBuffer;
    string filePath = configurationData.filePath;
    int hardwareCycleTime;
    bool doneParsing = false;
    Program program;

    const string BEGIN_SIMULATOR_STRING = "S(start)0";
    const string END_SIMULATOR_STRING   = "S(end)0.";
    const string BEGIN_PROGRAM_STRING   = "A(start)0";
    const string END_PROGRAM_STRING     = "A(end)0";

    // Open the file for reading
    ifstream inFile( filePath.c_str() );
    if( inFile.fail() )
    {
        cerr << "Error! Failed to open Meta-Data file." << endl;
        return false;
    }

    // Check first line
    getline( inFile, lineBuffer );
    if( lineBuffer != "Start Program Meta-Data Code:" )
    {
        cerr << "Error! Invalid Meta-Data file." << endl;
        if( inFile.is_open() )
        {
            inFile.close();
        }
        return false;
    }

    // Get each token delimited by semicolons
    while( !doneParsing )
    {
        // Ignore whitespace
        inFile >> std::ws;

        // Get the next operation
        getline( inFile, lineBuffer, ';' );

        // Check if at the end
        doneParsing = lineBuffer.find( END_SIMULATOR_STRING ) != string::npos;

        if( lineBuffer == BEGIN_PROGRAM_STRING )
        {
            program.clear_operations();
            hardwareCycleTime = 0;
            program.add_operation( lineBuffer, 0 );
        }
        else if( lineBuffer == END_PROGRAM_STRING )
        {
            program.add_operation( lineBuffer, 0 );
            programs_.push_back( program );
            programs_.back().prepare();
        }
        else if( ( lineBuffer != BEGIN_SIMULATOR_STRING ) && !doneParsing )
        {
            if( lineBuffer.find( "printer" ) != string::npos )
            {
                hardwareCycleTime = configurationData.printerCycleTime;
            }
            else if( lineBuffer.find( "monitor" ) != string::npos )
            {
                hardwareCycleTime = configurationData.monitorDisplayTime;
            }
            else if( lineBuffer.find( "keyboard" ) != string::npos )
            {
                hardwareCycleTime = configurationData.keyboardCycleTime;
            }
            else if( lineBuffer.find( "run" ) != string::npos )
            {
                hardwareCycleTime = configurationData.processorCycleTime;
            }
            else if( lineBuffer.find( "hard drive" ) != string::npos )
            {
                hardwareCycleTime = configurationData.hardDriveCycleTime;
            }
            else
            {
                cerr << "Error! Invalid Meta-Data file. "
                     << "Could not set operation hardware cycle!" << endl;

                if(inFile.is_open())
                {
                    inFile.close();
                }
                return false;
            }

            program.add_operation( lineBuffer, hardwareCycleTime );
        }

    }

    // Close the file
    if(inFile.is_open())
    {
        inFile.close();
    }

    return true;
}

void Simulator::run()
{
    const string ERROR_MESSAGE  =
        "No scheduling code specified. Aborting Simulator.";

    // Begin the simulation
    initialTime_ = chrono::high_resolution_clock::now();
    display( "Simulator program starting" );

    // Execute all operations in the queue
    switch( configurationData.schedulingCode )
    {
        case FIFO_P:
        {
            executeScheduling<FIFO_Q>();
            break;
        }

        case RR:
        {
            executeScheduling<RR_Q>();
            break;
        }

        default:
        {
            cerr << ERROR_MESSAGE << endl;
        }
    }

    // End the simulation
    display( "Simulator program ending" );
}

void Simulator::handleIO( const Operation& operation, int pid )
{
    string startMsg = "I/O: process " + to_string(pid) + " starting ";
    string endMsg = "Interrupt: process " + to_string(pid) + " done with ";

    if( operation.parameters().description == "hard drive" )
    {
        string type = operation.parameters().id == 'I' ? "input" : "output";

        display( startMsg + "hard drive " + type );
        wait( operation.parameters().duration );
        display( startMsg + "hard drive " + type );
    }
    else if( operation.parameters().description == "keyboard" )
    {
        display( startMsg + "keyboard input" );
        wait( operation.parameters().duration );
        display( endMsg + "keyboard input" );
    }
    else if( operation.parameters().description == "printer" )
    {
        display( startMsg + "printer output" );
        wait( operation.parameters().duration );
        display( endMsg + "printer output" );
    }
    else if( operation.parameters().description == "monitor" )
    {
        display( startMsg + "monitor output" );
        wait( operation.parameters().duration );
        display( endMsg + "monitor output" );
    }

    interrupts_.push(pid);
}

bool Simulator::setLoggingMode( const string &loggingMode )
{
    const string ERROR_MESSAGE =
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
        displayErrorMessage(ERROR_MESSAGE);
        return false;
    }

    return true;
}

bool Simulator::setSchedulingCode( const string &schedulingCode )
{
    const string ERROR_MESSAGE =
        "Invalid scheduling code specified. Could not set scheduling code";

    display( "OS: preparing all processes" );

    if( schedulingCode == "FIFO-P")
    {
        configurationData.schedulingCode = FIFO_P;
    }
    else if( schedulingCode == "RR" )
    {
        configurationData.schedulingCode = RR;
    }
    else
    {
        displayErrorMessage( ERROR_MESSAGE );
        return false;
    }

    return true;
}

bool Simulator::checkVersion() const
{
    const string ERROR_MESSAGE =
        "Invalid version specified in configuration file";

    if( configurationData.version != VERSION )
    {
        displayErrorMessage( ERROR_MESSAGE );
        return false;
    }

    return true;
}

void Simulator::displayErrorMessage( const string &message ) const
{
    cerr << "Error: " << message << endl;
}

Program Simulator::next(FIFO_Q* readyQueue)
{
    Program nextProgram = readyQueue->top();
    readyQueue->pop();
    return nextProgram;
}

Program Simulator::next(RR_Q* readyQueue)
{
    Program nextProgram = readyQueue->front();
    readyQueue->pop();
    return nextProgram;
}

template<typename Queue>
void Simulator::executeScheduling()
{
    int counter = 0;

    // Create a ready queue
    display( "OS: preparing all processes" );
    Queue* readyQueue = new Queue();
    for( Program program : programs_ )
    {
        program.prepare();
        readyQueue->push( program );
    }

    // Execute the programs according to RR scheduling
    while( !readyQueue->empty() || !suspendedPrograms_.empty() )
    {
        // Process interrupts
        while( !interrupts_.empty() )
        {
            int interrupt = interrupts_.front();
            interrupts_.pop();

            if( interrupt != 0 )
            {
                Program suspendedProgram = suspendedPrograms_.at( interrupt );
                suspendedPrograms_.erase( interrupt );

                suspendedProgram.prepare();
                readyQueue->push( suspendedProgram );
            }
        }

        // Execute programs in ready queue
        if( !readyQueue->empty() )
        {
            display( "OS: selecting next process" );
            Program program = next(readyQueue);

            if( program.process_control_block().processID == 0)
            {
                program.assign_pid(++counter);
            }

            // Execute the program until it is interupted
            executeProgram(&program);

            if( program.process_control_block().state == RUNNING )
            {
                program.prepare();
                readyQueue->push(program);
            }
        }
        else
        {
            display("OS Idle: Waiting for I/O to complete");
            wait(30); // 30 millisecond idle before checking again
        }
    }
}

void Simulator::executeProgram( Program *program )
{
    int pid = program->process_control_block().processID;
    Operation operation = program->step();
    char operationType = operation.parameters().id;

    // Create thread lambda
    auto ThreadStart = [this, operation, pid]()
    {
        handleIO(operation, pid);
    };

    program->run();

    // Start
    if( operationType == 'A' &&
        operation.parameters().description == "start")
    {
        display("OS: starting process " + to_string(pid));
        operation = program->step();
    }

    // I/O
    if( operationType == 'I' || operationType == 'O')
    {
        display("Process: " + to_string(pid) + ": starting I/O");
        thread IO_thread(ThreadStart);
        IO_thread.detach(); // async, do not block

        program->suspend();
        suspendedPrograms_[pid] = *program;
    }

    // Processing
    else if( operationType == 'P' )
    {
        display("Process " + to_string(pid) + ": processing action");
        int quantum = 0;
        while( !operation.completed() && interrupts_.empty() )
        {
            quantum++;

            operation = program->step();
            wait( operation.parameters().cycleTime );

            if( quantum == configurationData.quantum )
            {
                // Launch a quantum interrupt to stop execution of the program
                interrupts_.push(0);
                display("Interrupt: quantum expired");
            }
        }

        if( operation.completed() )
        {
            display("Process " + to_string(pid) + ": end processing action");
        }
    }

    // The last operation in a program is an exit operation
    if( program->operations_left() <= 1 &&
        program->process_control_block().state != SUSPENDED)
    {
        program->exit();
        display("OS: removing process " + to_string(pid));
    }
}

