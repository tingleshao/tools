#include <iostream>

#include <Timer.h>
//#include <C11Thread.h>
#include <DataBuffer.h>
#include <ThreadSafeQueue.h>
#include <ExtendedBuffer.h>
#include <BaseSocket.h>
#include <SocketServer.h>

using namespace std;

/**
 * Main test function
 **/
int main(void)
{
   if( !testTimer() )
   {
      cout << "Timer Unit Test Failed!" << endl;
      return 1;
   }
/*
   if( !testC11Thread() )
   {
      cout << "C11Thread Unit Test Failed!" << endl;
      return 1;
   }
*/
   cout << "Testing DataBuffer"<<endl;
   if( !testDataBuffer() )
   {
      cout << "DataBuffer Test Failed!" << endl;
      return 1;
   }
   cout << "Testing extended buffer"<<endl;
   if( !testExtendedBuffer())
   {
      cout << "ExtendedBuffer Test Failed!" << endl;
      return 1;
   }
   if( !test_ThreadSafeQueue())
   {
      cout << "Threadsafe Queue Test Failed!" << endl;
   }
   if( !testBaseSocket())
   {
      cout << "BaseSocket Test Failed!" << endl;
      return 1;
   }
   
   if( !testSocketServer())
   {
      cout << "SocketServer Test Failed!" << endl;
      return 1;
   }
   cout << "All tests completed successfully!" << endl;
   return 0;
}
