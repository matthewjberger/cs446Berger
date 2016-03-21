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

    // Open the log file if necessary
    if( configurationData.loggingMode | LOG_TO_BOTH | LOG_TO_FILE )
    {
        logFile_.open( configurationData.logFilePath );
    }
}

Simulator::~Simulator()
{
    if( logFile_.is_open() )
    {
        logFile_.close();
    }
}

void Simulator::display( const string &output )
{
    const auto timePassed = secondsPassed().count();

    if( configurationData.loggingMode |  LOG_TO_BOTH | LOG_TO_MONITOR )
    {
        cout << setw( LEADING_PRECISION ) << setfill( '0' )
             << fixed << timePassed << " - " << output << endl;
    }

    if( configurationData.loggingMode | LOG_TO_BOTH | LOG_TO_FILE )
    {
        logFile_ << setw( LEADING_PRECISION ) << setfill( '0' )
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
    this_thread::sleep_for( chrono::milliseconds( milliseconds ) );
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
            program.clearOperations();
        }
        else if( lineBuffer == END_PROGRAM_STRING )
        {
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

            program.addOperation( lineBuffer, hardwareCycleTime );
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
    display( "OS: preparing all processes" );

    // Execute all operations in the queue
    switch( configurationData.schedulingCode )
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

        default:
        {
            cerr << ERROR_MESSAGE << endl;
        }
    }

    // End the simulation
    display( "Simulator program ending" );
}

void Simulator::handleIO( const Operation& operation )
{
    string type;

    // Handle specific types of IO
    if( operation.description() == "hard drive" )
    {
        if( operation.id() == 'I' )
        {
            type = "input";
        }
        else if( operation.id() == 'O' )
        {
            type = "output";
        }

        // display output
        display( processText + "start hard drive " + type );
        wait( operation.duration() );
        display( processText + "end hard drive " + type );
    }
    else if( operation.description() == "keyboard" )
    {
        display( processText + "start keyboard input" );
        wait( operation.duration() );
        display( processText + "end keyboard input" );
    }
    else if( operation.description() == "printer" )
    {
        display( processText + "start printer output" );
        wait( operation.duration() );
        display( processText + "end printer output" );
    }
    else if( operation.description() == "monitor" )
    {
        display( processText + "start monitor output" );
        wait( operation.duration() );
        display( processText + "end monitor output" );
    }
}

void Simulator::handleOperation( const Operation& operation )
{
    switch( operation.id() )
    {
        // Processing
        case 'P':
        {
            display( processText + "start processing action" );
            wait( operation.duration() );
            display( processText + "end processing action" );
            break;
        }

        // I/O
        case 'I':
        case 'O':
        {
            // Create a new thread for the IO operation
            thread IO_Thread(
                    [ this, operation ]()
                    {
                        // Run this function when the thread executes
                        handleIO( operation );
                    });

            // Join it to wait for the thread to complete
            if( IO_Thread.joinable() )
            {
                IO_Thread.join();
            }

            break;
        }
    }
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

    if( schedulingCode == "FCFS" || schedulingCode == "FIFO" )
    {
        configurationData.schedulingCode = FCFS;
    }
    else if( schedulingCode == "SJF" )
    {
        configurationData.schedulingCode = SJF;
    }
    else if( schedulingCode == "SRTF_N" )
    {
        configurationData.schedulingCode = SRTF_N;
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

void Simulator::executeFCFS()
{
    while(!programs_.empty())
    {
        displayLoadProcessText();
        programs_.front().run();
        for( auto operation : programs_.front().operations() )
        {
            handleOperation( operation );
        }

        displayRemoveProcessText();
        programs_.front().exit();
        programs_.pop_front();
    }
}

void Simulator::executeSJF()
{
    // Same as FCFS except the programs are
    // sorted by their duration in the PCB

    // Declare how we will sort the programs
    auto SortByDuration =
    []( const Program &program_1, const Program &program_2 ) -> bool
    {
        return ( program_1.duration() < program_2.duration() );
    };

    // Execute the sort in place
    programs_.sort( SortByDuration );

    // Execute in order
    executeFCFS();
}

void Simulator::executeSRTFN()
{
}

void Simulator::displayLoadProcessText()
{
    processCount_++;
    processText = "Process " + to_string( processCount_ ) + ": ";
    display( "OS: selecting next process" );
    display( "OS: starting process " + to_string( processCount_ ) );
}

void Simulator::displayRemoveProcessText()
{
    display( "OS: removing process " + to_string( processCount_ ) );
}
