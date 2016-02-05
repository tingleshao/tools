#include <iostream>

using namespace std;

#include <Timer.h>
#include <C11Thread.h>
#include <DataBuffer.h>
#include <ThreadSafeQueue.h>
#include <ExtendedBuffer.h>
#include <BaseSocket.h>

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
   if( !testC11Thread() )
   {
      cout << "C11Thread Unit Test Failed!" << endl;
      return 1;
   }
   if( !testDataBuffer() )
   {
      cout << "DataBuffer Test Failed!" << endl;
      return 1;
   }
   if( !test_ThreadSafeQueue())
   {
      cout << "Threadsafe Queue Test Failed!" << endl;
   }
   if( !testExtendedBuffer())
   {
      cout << "ExtendedBuffer Test Failed!" << endl;
      return 1;
   }
   if( !testBaseSocket())
   {
      cout << "BaseSocket Test Failed!" << endl;
      return 1;
   }
   
   cout << "All tests completed successfully!" << endl;
   return 0;
}
