//==============================================================================
/*!\file Timer.h
 * \brief AWARE Camera Interface Wrapper
 *
 * \author S. D. Feller 2013
 *
 * Timer data formats
 *
 * SMPTETime - structure with hour, minute, second and frame fields
 * double    - represents time as UTCSeconds.utc microseconds
 * timeval   - structure with 32bit tv_sec and 32 bit tv_usec
 * int64_t   - 
 *  
*/
//==============================================================================
#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <string>

#define TIMER_STEP 1e6                      //!< Used for tracking FPS
/**
 * \brief Timecode structure that represents time as hhmmssff
 *
 * http://faculty.spokanefalls.edu/InetShare/AutoWebs/steveg/SmpteMadeSimple.pdf
 **/
typedef struct
{
   uint8_t hour;                            //!< Hour (0-23)
   uint8_t minute;                          //!< Minute(0-59)
   uint8_t second;                          //!< Second(0-59)
   uint8_t frame;                           //!< Frame(0-99)
} SMPTETime;

/**
 * \brief Timer class for system metrics and measuring time
 *
 **/
class Timer
{
   private:
     double  startTime = 0;                 //!< reference time point
     double  stopTime = 0;                  //!< preallocated current time
     int64_t timeCodeOffset = 0;            //!< offset from system time to global timecode
     double  fps=30;                        //!< Number of frames per second a

   public:
      Timer();
      ~Timer();
      SMPTETime getTimeCode();
      void updateTimeCodeOffset( int64_t refTimeCode);
      int64_t getTimeCodeOffset();
      void    setFPS( double rate );
      int     getFPS( void );

      void    start();                     
      double  elapsed();                     
};

//Support functions
double getTime();
uint64_t getIntTime();
double convertTimeValToDouble( timeval tv );
SMPTETime   convertTimeValToSMPTE( timeval tv, int fps ); 
timeval     convertDoubleToTimeVal( double dTime );
SMPTETime   convertDoubleToSMPTE( double dTime, int fps );
std::string convertTimeValToString( timeval tv, int fps );
int64_t     convertSMPTEToTimeCode( SMPTETime time );
int64_t     convertTimeValToTimeCode( timeval tv, int fps );
int64_t     convertDoubleToTimeCode( double dTime, int fps );
uint64_t    convertDoubleToTimeStamp( double dTime );

timeval TimevalSum(const timeval &tv1, const timeval &tv2);
bool TimevalGreater(const timeval &tv1, const timeval &tv2);
timeval TimevalScale(const timeval &tv, double scale);
timeval TimevalDiff(const timeval &tv1, const timeval &tv2);
timeval TimevalNormalize(const timeval &in_tv);
static inline void timevalNormalizeInPlace(timeval &in_tv);



//Functional test for the timer
int testTimer(void);
#endif
