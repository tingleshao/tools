//==============================================================================
// A simple C++ wrapper class for C++11 threads
//
//==============================================================================

#include <iostream>
#include <sys/types.h>
#include <chrono>
#include <thread>
#include <vector>

#include "C11Thread.h"

using namespace std;

/**
 * \brief  Destructor
 *
 * This destructor waits for the internal thread to complete (join) 
 * before self-destructing. 
 **/
C11Thread::~C11Thread()
{
   Join();
}

/** 
 * \brief Function called to start thread execution
 *
 * \param [in] arg generic pointer that represents shared data
 * \return std::thread object for this threads
 **/
bool C11Thread::Start( bool * runFlag ) 
{
   if( threadObj != NULL ) {
      cout << "C11Thread threadObject already allocated!" << endl;
      return false;
   }

   runPtr = runFlag;
   running = true;

   threadObj = new std::thread;

   *threadObj = std::thread(&C11Thread::Execute, this);
   initialized = true;
   return true;
}

/**
 * \brief Execution function that starts thread processing
 *
 * \param [in] arg generic pointer to shared data
 *
 * This function is entry point into thread operation. This
 * function exits when the runPtr or running flag are set to false.
 **/
void C11Thread::Execute() 
{
   while( running ) 
   {
      //Adjust based on the runPtr, if it is defined.
      if( runPtr != NULL ) {
         running = *runPtr;
      }

      //If we're still running enter our loop
      if( running ) {
         mainLoop();
      }
   }

   


   return;
}

/** 
 * \brief Main execution loop for the thread
 *
 * This function is the main processing loop. It needs to be overwritten for
 * each class instantiation. In the base class, it sleep for 1000uSecs
 **/
void C11Thread::mainLoop() 
{
   std::cout << threadObj->get_id() <<": C11Thread mainLoop"<<endl;
   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

/**
 * \brief Waits for the thread to complete before returning
 *
 * This call is implmenets the std::thread::join for the class
 **/
bool C11Thread::Join()
{
   std::lock_guard<std::mutex> guard (threadMutex);
   if( threadObj == NULL ) {
      return false;
   }

   if( !threadObj->joinable()) {
      return false;
   }

   threadObj->join();

   delete threadObj;
   threadObj = NULL;

   return true;
}

/**
 * \brief forces the thread to stop running
 **/
void C11Thread::Stop() 
{
   running = false;
}

/**
 * \brief Test function
 **/
bool testC11Thread(void)
{
   bool rc = true;
   C11Thread cThread;

   cout << "Test Stop function"<<endl;
   cThread.Start(NULL);
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   cThread.Stop();
   cThread.Join();

   cout << "Test running flag"<<endl;
   static bool running = true;
   cThread.Start(&running);
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   std::cout << "Running being set to false!" << endl;
   running = false;
   rc = cThread.Join();
   if( !rc ) {
      std::cout << "Unable to join thread!"<<std::endl;
   }
   std::cout << "Joined"<<std::endl;

   //Vector tests (timed)
   size_t threadCount = 100;
   running = true;
   std::vector<C11Thread> threadVect(threadCount);

   cout << "Testing vector "<<endl << endl;
   //Spawn threadCount threads
   for( int i = 0; i < threadCount; i++ ) {
      threadVect[i].Start(&running);
   }


   //Stop running
   running = false;

   //Join all
   for( int i = 0; i < threadCount; i++ ) {
      threadVect[i].Join();
   }

   //ReSpawn threadCount threads
   for( int i = 0; i < threadCount; i++ ) {
      threadVect[i].Start(&running);
   }

   //Stop threadCount threads
   for( int i = 0; i < threadCount; i++ ) {
      threadVect[i].Stop();
   }
   
   return true;

}

