// Program Information ////////////////////////////////////////////////////////
/**
 * @file SimpleTimer.cpp
 * 
 * @brief Implementation file for SimpleTimer class
 * 
 * @author Michael Leverington
 *
 * @details Implements member methods for timing
 *
 * @version 1.00 (11 September 2015)
 *
 * @Note Requires SimpleTimer.h.
 * 
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef SIMPLETIMER_CPP
#define SIMPLETIMER_CPP

// Header files ///////////////////////////////////////////////////////////////

#include "SimpleTimer.h"

/**
 * @brief Default constructor
 *
 * @details Constructs Timer class
 *          
 * @param None
 *
 * @note set running flag to false
 */
SimpleTimer::SimpleTimer
       (
        // no parameters
       )
   {
    running = false;
   }

/**
 * @brief Default constructor
 *
 * @details Destructs Timer class
 *          
 * @param None
 *
 * @note No data to clear
 */
SimpleTimer::~SimpleTimer
       (
        // no parameters
       )
   {
    // no action here 
   }

/**
 * @brief Start control
 *
 * @details Takes initial time data
 *          
 * @param None
 *
 * @note None
 */
void SimpleTimer::start
       (
        // no parameters
       )
   {
    gettimeofday( &startData, NULL );

    running = true;

    dataGood = false;
   }

/**
 * @brief Stop control
 *
 * @details Takes final time data, calculates duration
 *          
 * @param None
 *
 * @note None
 */
void SimpleTimer::stop
       (
        // no parameters
       )
   {
    if( running )
       {
        gettimeofday( &endData, NULL );

        running = false;

        dataGood = true;
       }

    else
       {
        dataGood = false;
       }
   }

void SimpleTimer::getElapsedTime
       (
        char *timeStr
       )
   {
    int low, high, index = 0;
    char temp;

    if( dataGood )
       {
        secTime = endData.tv_sec - startData.tv_sec;
        microSecTime = endData.tv_usec - startData.tv_usec;

        if( microSecTime < 0 )
           {
            microSecTime += 1000000;

            secTime -= 1;
           }

        while( microSecTime > 0 )
           {
            timeStr[ index ] = char( microSecTime % 10 + '0' );

            microSecTime /= 10;
    
            index++;
           }

        while( index < 6 )
           {
            timeStr[ index ] = '0';

            index++;
           }

        timeStr[ index ] = RADIX_POINT;

        index++;

        if( secTime == 0 )
           {
            timeStr[ index ] = '0';
    
            index++;
           }

        while( secTime > 0 )
           {
            timeStr[ index ] = char( secTime % 10 + '0' );

            secTime /= 10;

            index++;
           }

        timeStr[ index ] = NULL_CHAR;

        low = 0; high = index - 1;

        while( low < high )
           {
            temp = timeStr[ low ];
            timeStr[ low ] = timeStr[ high ];
            timeStr[ high ] = temp;

            low++; high--;
           }
       }

    else
       {
        strcpy( timeStr, "No Data" );
       }
   }



#endif // ifndef SIMPLETIMER_CPP





