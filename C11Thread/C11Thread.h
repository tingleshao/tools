//==============================================================================
// A simple C++ wrapper class for c++11 threads
//
// The intended usage of this class is via inheritence--it does nothing itself.
// First, create a new class that inherits from C11Thread. Overide the Setup and
// Execute methods. The Setup method is intended to be executed once. The
// Execute method is the actual thread (it contains the thread's execution
// loop, and does not exit except under special (i.e. fault) circumstances).
//==============================================================================
#pragma once
#include <thread>
#include <mutex>

//************************************************************
/**
 *!\brief A simple C++ wrapper class for POSIX threads.
 *
 * The intended usage of this class is via inheritence--it does nothing itself.
 * First, create a new class that inherits from jthread. Overide the Setup and
 * Execute methods. The Setup method is intended to be executed once. The
 * Execute method is the actual thread (it contains the thread's execution
 * loop, and does not exit except under special (i.e. fault) circumstances).
 **/
//************************************************************
class C11Thread
{
    private:
       std::mutex threadMutex;            //!< Mutex to signal closure
       bool initialized = false;          //!< Flag to indicate thread is operational
       std::thread * threadObj = NULL;    //!< Thread variable
       bool * runPtr = NULL;              //!< Flag to stop running by a shared pointer
       bool running = false;              //!< Internal flag to stop execution due to function call

       virtual void Execute( void * arg );

    protected:

    public:
       ~C11Thread();
       bool Start( void * arg = NULL, bool * runFlag=NULL );
       bool Join( void );
       void Stop();
       virtual void mainLoop();

    protected:
};

bool testC11Thread();
