//***********************************************************
/*!\file Timer.cpp
 * \brief Timer class provides timing functionality for testing
 * program performance
 *
 * \author S. D. Feller 2014
 *************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <thread>
#include <chrono>

//#include <curl/curl.h>
//#include <JsonBox.h>
#include "Timer.h"

//============================================================
/*!\brief Timer constructor
 */
//============================================================
Timer::Timer()
{
   //Initialize random number generator
   srand( time(NULL));
   start();
}

//***********************************************************
/*!\brief Timer destructor
 */
//************************************************************/
Timer::~Timer()
{
}

//***********************************************************
/*!\brief Sets the number of fps in timer
 */
//************************************************************
void Timer::setFPS( double rate )
{
   fps = rate;
}

//***********************************************************
/*!\brief returns the number of fps in timer
 */
//************************************************************
int Timer::getFPS( )
{
   return fps;
}


//***********************************************************
/**
 *!\brief returns the current timecode
 *
 * This is an internal function since it will eventually calculate
 * frame rate and other information
 **/
//************************************************************
SMPTETime Timer::getTimeCode( )
{
   double now = getTime() + timeCodeOffset;

   if( fps == 0 ) {
      fprintf(stderr, "Timer::getTimeCode unkown fps.\n");
   }
   return( convertDoubleToSMPTE( now, fps ));
}


//***********************************************************
/*!\brief returns the timeCode offset to match system time with global time
 */
//************************************************************
void Timer::updateTimeCodeOffset( int64_t refTime )
{
   //Get current time
   timeval tv;
   gettimeofday(&tv, NULL);

   timeCodeOffset = refTime - convertTimeValToTimeCode( tv, fps );

   return;
}

   
//***********************************************************
/*!\brief returns the timeCode offset to match system time with global time
 */
//************************************************************
int64_t Timer::getTimeCodeOffset()
{
   return timeCodeOffset;
}

   
//***********************************************************
/*!\brief starts timer
 *
 * Resets the start time and becomes the new point of reference
 */
//************************************************************/
void Timer::start()
{
   timeval tv;
   gettimeofday( &tv, NULL );
   startTime = convertTimeValToDouble( tv );
   return;
}

//***********************************************************
/*!\brief starts timer
 *
 * Resets the start time and becomes the new point of reference
 */
//************************************************************/
double Timer::elapsed()
{
   timeval tv;
   gettimeofday( &tv, NULL );
   stopTime = convertTimeValToDouble( tv );

   return stopTime - startTime;
}

//***********************************************************
/*!\brief converts a timeval struct to double
 */
//************************************************************/
double convertTimeValToDouble( timeval tv )
{
   return (double)tv.tv_sec + (double)((int)tv.tv_usec)/double(TIMER_STEP);
}

//***********************************************************
/*!\brief converts a double to a timeval struct
 */
//************************************************************/
timeval convertDoubleToTimeVal( double dTime )
{
   timeval tv;
   tv.tv_sec = (time_t)dTime;
   tv.tv_usec = (suseconds_t)((dTime-(int)tv.tv_sec) * TIMER_STEP ); 

   return tv;
}

//***********************************************************
/**
 *!\brief converts a double to an SMPTE time.
 *
 * \param [in] dTime time to convert
 * \param [in] fps number of frames per second (used to convert decimal time to a frame)
 *
 **/
//***********************************************************
SMPTETime convertDoubleToSMPTE( double dTime , int fps )
{
   timeval tv = convertDoubleToTimeVal( dTime );
   SMPTETime smpte = convertTimeValToSMPTE( tv, fps );

   return smpte;
}

//***********************************************************
/*!\brief converts a SMPTE to Timecode (hhmmssff)
 */
//************************************************************/
int64_t convertSMPTEToTimeCode( SMPTETime smpte )
{
   return smpte.hour*1e6 + smpte.minute*1e4+smpte.second*1e2+smpte.frame;
}

//***********************************************************
/*!\brief converts a timeval structure to Timecode (hhmmssff)
 */
//************************************************************/
int64_t convertTimeValToTimeCode( timeval tv, int fps )
{
   SMPTETime smpte = convertTimeValToSMPTE( tv, fps );

   return convertSMPTEToTimeCode( smpte );
}
  
//***********************************************************
/**
 *!\brief converts a double to an SMPTE time.
 *
 * \param [in] tv time  to convert
 * \param [in] fps number of frames per second (used to convert decimal time to a frame)
 *
 **/
//***********************************************************
SMPTETime convertTimeValToSMPTE( timeval tv, int fps )
{
   SMPTETime smpte;
   memset( &smpte, 0, sizeof( SMPTETime ));
   //Convert timestamp to current time
   struct tm * tmptr = localtime( &tv.tv_sec );

   smpte.hour    = tmptr->tm_hour;
   smpte.minute  = tmptr->tm_min;
   smpte.second  = tmptr->tm_sec;
   smpte.frame   = fps * (double(tv.tv_usec)/double(TIMER_STEP));


   return smpte;
}

//***********************************************************
/**
 * \brief Converts the given timeval and int into a a string
 **/
//***********************************************************
std::string convertTimeValToString( timeval tv, int fps )
{
   char buffer[256];

   SMPTETime smpte = convertTimeValToSMPTE( tv, fps );

   sprintf( buffer, "%02d:%02d:%02d:%02d\n"
          , smpte.hour
          , smpte.minute
          , smpte.second
          , smpte.frame
          );

   std::string result;
   result.assign( buffer );

   return result;
}

//***********************************************************
/**
 * \brief Converts the given double into a string with fps
 **/
//***********************************************************
std::string convertDoubleToString( double dTime,  int fps )
{
   timeval tv = convertDoubleToTimeVal( dTime );

   return convertTimeValToString( tv, fps ) ;
}


//***********************************************************
/**
 * \brief Converts a double to a TimeCode
 **/
//***********************************************************
int64_t convertDoubleToTimeCode( double dTime,  int fps )
{
   timeval tv = convertDoubleToTimeVal( dTime );

   return convertTimeValToTimeCode( convertDoubleToTimeVal( dTime ), fps) ;
}

//***********************************************************
/**
 * \brief Gets the current time as a double
 **/
//***********************************************************
double getTime()
{
   timeval tv;
   gettimeofday(&tv, NULL);
   
   return convertTimeValToDouble( tv );
}


//***********************************************************
/*!\brief Returns the current timestamp 
 *
 * \return 64bit timestamp
 *
 * This is an internal function since it will eventually calculate
 * frame rate and other information
 */
//************************************************************/
uint64_t convertDoubleToTimeStamp( double dTime )
{
   return (uint64_t)(dTime * 1e8);
}

/**
 *!\brief calculates the sum of two time values
 &
 * Calcs the sum of tv1 and tv2.  Returns the sum in a timeval struct.
 * Calcs negative times properly, with the appropriate sign on both tv_sec
 * and tv_usec (these signs will match unless one of them is 0).
 * NOTE: both abs(tv_usec)'s must be < 1000000 (ie, normal timeval format)
 *
 * borrowed copiously from vrpn (thanks Russell )
 **/
timeval TimevalSum(const timeval &tv1, const timeval &tv2)
{
    timeval tvSum = tv1;

    tvSum.tv_sec += tv2.tv_sec;
    tvSum.tv_usec += tv2.tv_usec;

    // do borrows, etc to get the time the way i want it: both signs the same,
    // and abs(usec) less than 1e6
    if (tvSum.tv_sec > 0) {
        if (tvSum.tv_usec < 0) {
            tvSum.tv_sec--;
            tvSum.tv_usec += 1000000;
        }
        else if (tvSum.tv_usec >= 1000000) {
            tvSum.tv_sec++;
            tvSum.tv_usec -= 1000000;
        }
    }
    else if (tvSum.tv_sec < 0) {
        if (tvSum.tv_usec > 0) {
            tvSum.tv_sec++;
            tvSum.tv_usec -= 1000000;
        }
        else if (tvSum.tv_usec <= -1000000) {
            tvSum.tv_sec--;
            tvSum.tv_usec += 1000000;
        }
    }
    else {
        // == 0, so just adjust usec
        if (tvSum.tv_usec >= 1000000) {
            tvSum.tv_sec++;
            tvSum.tv_usec -= 1000000;
        }
        else if (tvSum.tv_usec <= -1000000) {
            tvSum.tv_sec--;
            tvSum.tv_usec += 1000000;
        }
    }

    return tvSum;
}

/**
 *!\brief perform normalization of a timeval
 * XXX this still needs to be checked for errors if the timeval
 * or the rate is negative
 **/
static inline void timevalNormalizeInPlace(timeval &in_tv)
{
    const long div_77777 = (in_tv.tv_usec / 1000000);
    in_tv.tv_sec += div_77777;
    in_tv.tv_usec -= (div_77777 * 1000000);
}

/**
 *!\brief perform normalization of a timeval
 **/
timeval TimevalNormalize(const timeval &in_tv)
{
    timeval out_tv = in_tv;
    timevalNormalizeInPlace(out_tv);
    return out_tv;
}

/**
 *!\brief Calcs the diff between tv1 and tv2.  
 *
 *\return the diff in a timeval struct.
 * Calcs negative times properly, with the appropriate sign on both tv_sec
 * and tv_usec (these signs will match unless one of them is 0)
 **/
timeval TimevalDiff(const timeval &tv1, const timeval &tv2)
{
    timeval tv;

    tv.tv_sec = -tv2.tv_sec;
    tv.tv_usec = -tv2.tv_usec;

    return TimevalSum(tv1, tv);
}

/**
 *!\brief multiplies the timevale by the given structure
 **/
timeval TimevalScale(const timeval &tv, double scale)
{
    timeval result;
    result.tv_sec = (long)(tv.tv_sec * scale);
    result.tv_usec =
        (long)(tv.tv_usec * scale + fmod(tv.tv_sec * scale, 1.0) * 1000000.0);
    timevalNormalizeInPlace(result);
    return result;
}

/**
 *!\brief compares two time values
 *\return 1 if tv1 is greater than tv2;  0 otherwise
 **/
bool TimevalGreater(const timeval &tv1, const timeval &tv2)
{
    if (tv1.tv_sec > tv2.tv_sec) return 1;
    if ((tv1.tv_sec == tv2.tv_sec) && (tv1.tv_usec > tv2.tv_usec)) return 1;
    return 0;
}

/**
 *!\brief checks if two timevals are equal 
 *
 *\return 1 if tv1 is equal to tv2; 0 otherwise
 **/
bool TimevalEqual(const timeval &tv1, const timeval &tv2)
{
    if (tv1.tv_sec == tv2.tv_sec && tv1.tv_usec == tv2.tv_usec)
        return true;
    else
        return false;
}

/**
 * \brief Sleeps for the given amount of time
 *
 * \param [in] double time to sleep in seconds
 **/
void ASleep( double time ) {
   size_t msec = time*1e3;
   std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}


//***********************************************************
/*!\brief functional test for this class
 */
//***********************************************************
int testTimer()
{
   printf("\n\nRegression test for Timer class\n");
   Timer timer;
//   timer.setFPS(30);

   //Check core functions
   for( int i = 1; i < 1e6; i += 1e6/30 )
   {
      double value = double(i)/1e6;
      std::string timeStr = convertDoubleToString( value, 30 );
      printf( "%d - %lf\t Value: %s\n", i, value, timeStr.c_str());
   }


   timer.start();
   usleep(10);
   double result = timer.elapsed();
   printf("We should be close to 10 microseconds (.000010): %lf\n", result );
   if(( result < .000010 )||(result > 00.000300 ))
   {
      printf("Timer is off!\n");
      return -1;
   }

   timer.start();

   sleep(2);
   result = timer.elapsed();
   printf("We should be close to 2.000010: %lf\n", result );
   if(( result < 2.000010 )||(result > 2.000300 ))
   {
      printf("Timer is off!\n");
      return -1;
   }

   //Get timestamp
   int64_t tstamp = convertDoubleToTimeStamp( getTime());
   printf("Current timestamp: %ld\n", tstamp );

   timer.start();
   result = timer.elapsed();
   printf("Timer since second start (can be 0 if less than 1 msec): %lf\n", result );

   double dTime = getTime();
   SMPTETime smpte = convertDoubleToSMPTE( dTime, 30 );
   printf( "SMPTETme: %2d:%2d:%2d, frame: %d\n"
         , smpte.hour
         , smpte.minute
         , smpte.second
         , smpte.frame 
         );

   std::string timeStr = convertTimeValToString( convertDoubleToTimeVal( dTime ), 30 );
   printf( "StringTime: %s", timeStr.c_str());

   int64_t timecode = convertTimeValToTimeCode( convertDoubleToTimeVal( dTime ), 30);
   printf("TimeCode: %ld\n", timecode );


   return 1;
}

