//******************************************************************************
// A simple C++ wrapper class for C++11 threads
//
//******************************************************************************

#include <iostream>
#include <sys/types.h>
#include <chrono>
#include <thread>
#include <vector>

#include "AThread.h"

using namespace std;
namespace atl
{
   
   /**
    * \brief  Destructor
    *
    * This destructor waits for the internal thread to complete (join) 
    * before self-destructing. 
    **/
   AThread::~AThread()
   {
      Join();
   }
   
   /** 
    * \brief Function called to start thread execution
    *
    * \param [in] runFlag boolean pointer to terminate thread execution
    * \return std::thread object for this threads
    **/
   bool AThread::Start( bool * runFlag ) 
   {
      if( threadObj != NULL ) {
         cout << "AThread threadObject already allocated!" << endl;
         return false;
      }
   
      runPtr = runFlag;
   
      threadObj = new std::thread;
   
      *threadObj = std::thread(&AThread::EntryPoint, this);
      initialized = true;
      return true;
   }
   
   /**
    * \brief EntryPointer to transfer control to the run method
    **/
   void AThread::EntryPoint(AThread * pthis) 
   {
   //    AThread * ptr = (AThread *) pthis;
       pthis->Execute();
   
       return;
   }
   
   /**
    * \brief Function to start thread execution.
    **/ 
   void AThread::Run(void) 
   {
      Execute();
   }
   
   /**
    * \brief Execution function that starts thread processing
    *
    * This function is entry point into thread operation. This
    * function exits when the runPtr or running flag are set to false.
    **/
   void AThread::Execute() 
   {
      running = true;
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
   void AThread::mainLoop(void) 
   {
      std::cout << threadObj->get_id() <<": AThread mainLoop"<<endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   }
   
   /**
    * \brief Waits for the thread to complete before returning
    *
    * This call is implmenets the std::thread::join for the class
    **/
   bool AThread::Join()
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
   void AThread::Stop() 
   {
      running = false;
   }
   
   /**
    * \brief Test function
    **/
   bool testAThread(void)
   {
      bool rc = true;
      AThread cThread;
   
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
      size_t threadCount = 50;
      running = true;
      std::vector<AThread> threadVect(threadCount);
   
      cout << "Testing vector "<<endl << endl;
      //Spawn threadCount threads
      for( uint16_t i = 0; i < threadCount; i++ ) {
         threadVect[i].Start(&running);
      }
   
   
      //Stop running
      running = false;
   
      //Join all
      for( uint16_t i = 0; i < threadCount; i++ ) {
         threadVect[i].Join();
      }
   
      //ReSpawn threadCount threads
      for( uint16_t i = 0; i < threadCount; i++ ) {
         threadVect[i].Start(&running);
      }
   
      //Stop threadCount threads
      for( unsigned int i = 0; i < threadCount; i++ ) {
         threadVect[i].Stop();
      }
      
      return true;
   
   }
}

