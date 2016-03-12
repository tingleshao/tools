#include <iostream>

#include <ATimer.h>
#include <AThread.h>
#include <BaseMetadata.h>
#include <ImageMetadata.h>
#include <BaseBuffer.h>
#include <DataBuffer.h>
#include <ExtendedBuffer.tcc>
#include <BaseSocket.h>
#include <SocketServer.h>

using namespace std;
using namespace atl;

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
   if( !testAThread() )
   {
      cout << "C11Thread Unit Test Failed!" << endl;
      return 1;
   }
   cout << "Testing BaseMetadata"<<endl;
   if( !atl::testBaseMetadata() )
   {
      cout << "BaseMetadata Test Failed!" << endl;
      return 1;
   }
   cout << "Testing ImageMetadata"<<endl;
   if( !atl::testImageMetadata() )
   {
      cout << "ImageMetadata Test Failed!" << endl;
      return 1;
   }
   cout << "Testing BaseBuffer"<<endl;
   if( !atl::testBaseBuffer() )
   {
      cout << "BaseBuffer Test Failed!" << endl;
      return 1;
   }
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
