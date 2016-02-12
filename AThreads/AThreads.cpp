//==============================================================================
// A simple C++ wrapper class for POSIX threads.
//
// To use this class a new class is created which inherits from the jthread
// class defined here. This new class definition overrides the Setup() and
// Execute() methods here (which are defined as virtual functions) with the
// code that will perform one-time setup actions, and the main execution loop
// of the thread itself, respectively. When the Start() method is called it
// will invoke the user-defined Start() method (if any) and then the Execute()
// method. A user-defined parameter may be passed in via the Start() method,
// and two versions are provided to support this. A Join() method is also
// included.
//
// This is based partly on the work of an unknown author and extended by JMH.
//
// Note that the compile will complain about the lack of return statements in
// the functions Run and EntryPoint. This is expected. Because of the thread
// function must be handled in an indirect fashion since it can't be a normal
// member method of the class.
//------------------------------------------------------------------------------
// This software is part of the IDR-R/HAL system developed at the University of
// Arizona for Duke Univeristy in support of Phase I of the DARPA MOSAIC
// project.
//------------------------------------------------------------------------------
// J. M. Hughes August-October 2011
//==============================================================================

#include <iostream>
#include <sys/types.h>
#include <pthread.h>

#include "AThreads.h"

using namespace std;


jthread::jthread() {}

/**
 *!\brief Function to start independent thread processing with no thread parameters.
 *
 * This function is the equivalent of the pthread_create function in linux. 
 * Functions no longer accessible.
 **/
int jthread::Start()
{
    int code = pthread_create(&ThreadId_, NULL, EntryPoint, (void *) this);

    return code;
}


/**
 *!\brief Function to start independent thread processing with a parameter.
 *
 * \param [in] arg pointer to a shared data structure between the thread and the calling process
 *
 * This function is the equivalent of the pthread_create function in linux. 
 * Functions no longer accessible.
 **/
int jthread::Start(void *arg)
{
    m_running = true;
    m_runningPtr = (bool *)arg;

    int code = pthread_create(&ThreadId_, NULL, EntryPoint, (void *) this);

    return code;
}

/**
 * \brief sets running flag to false
 **/
void jthread::Stop() 
{
   m_running = false;
}

/**
 *!\brief Function to return the thread ID
 **/
pthread_t jthread::GetThreadID()
{
    return ThreadId_;
}


/**
 *!\brief Function to detach the thread
 *
 * This function calls pthread_detach
 **/
void jthread::Detach()
{
    pthread_detach(ThreadId_);
}


/**
 *!\brief Function to join the thread
 *
 * This function call pthread_join
 **/
void jthread::Join()
{
    pthread_join(ThreadId_, NULL);
}


/**
 *!\brief Function to exit the thread
 *
 * This function calls pthread_exit(NULL)
 **/
void jthread::Exit()
{
    pthread_exit(NULL);
}


/**
 *!\brief Function to start thread execution (with setup function)
 *
 * \param [in] arg pointer to a shared data structure between the thread and the calling process
 **/
int jthread::Run(void)
{
    Setup();
    Execute();

    return 1;
}


/**
 *!\brief Function that can be overwritten to add to procprocessing in thread startup.
 **/
void jthread::Setup()
{
    // Overide to implement setup, do not use this as the main thread
    // body (i.e. the main loop)--let it execute just once.
}

/**
 *!\brief Function that can be overwritten to create the main processing loop
 **/
void jthread::Execute()
{
    // Override this method with main thread loop
}


/*static */

/**
 *!\brief Internal Function that transfers control to the run() method
 * 
 * The thread's entry function cannot be a normal member function of a class
 * when used with pthread_create(). It can, however, be a static member
 * function of a class.
 **/
void *jthread::EntryPoint(void *pthis)
{
   jthread *pt = (jthread *) pthis;
   pt->Run();

   return NULL;
}



