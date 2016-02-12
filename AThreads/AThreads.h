//==============================================================================
// A simple C++ wrapper class for POSIX threads.
//
// The intended usage of this class is via inheritence--it does nothing itself.
// First, create a new class that inherits from jthread. Overide the Setup and
// Execute methods. The Setup method is intended to be executed once. The
// Execute method is the actual thread (it contains the thread's execution
// loop, and does not exit except under special (i.e. fault) circumstances).
//
//------------------------------------------------------------------------------
// This software is part of the IDR-R/HAL system developed at the University of
// Arizona for Duke Univeristy in support of Phase I of the DARPA MOSAIC
// project.
//------------------------------------------------------------------------------
// J. M. Hughes August-October 2011
//==============================================================================

#ifndef H_THREAD
#define H_THREAD

#include <pthread.h>

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
class jthread
{
    private:
    int       ret;                  //!< Return code
    pthread_t ThreadId_;            //!< ID of this thread
    bool      m_running = false;    //!<Flag to indicate we are done running
    bool *    m_runningPtr = NULL;  //!<Flag pointer to tell process to stop execution

    public:
    jthread();
    int Start(void *arg);
    int Start();
    void Stop();
    pthread_t GetThreadID();
    void Detach();
    void Join();
    void Exit();

    protected:
    int Run(void);
    static void *EntryPoint(void*); // note: static function

    virtual void Setup();
    virtual void Execute(void);
};

#endif
