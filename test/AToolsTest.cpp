#include <iostream>

#include <ATimer.h>
#include <AThread.h>
#include <BaseContainerMetadata.h>
#include <BaseContainerArrayMetadata.h>
#include <BaseContainer.h>
#include <ImageMetadata.h>
#include <BaseBuffer.h>
#include <DataBuffer.h>
#include <ExtendedBuffer.tcc>
#include <BaseSocket.h>
#include <SocketServer.h>
#include <TSArray.tcc>

using namespace std;
using namespace atl;

/**
 * Main test function
 **/
int main(void)
{
   /*
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
   */
   cout << "Testing BaseContainerMetadata"<<endl;
   if( !atl::testBaseContainerMetadata() )
   {
      cout << "BaseMetadata Test Failed!" << endl;
      return 1;
   }
   cout << "Testing BaseContainer"<<endl;
   if( !atl::testBaseContainer() )
   {
      cout << "Test BaseContainer Failed!" << endl;
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
   /*
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
   */

   cout << "Testing TSArray"<<endl;
   if( !testTSArray()) {
      std::cout << "TSArray test failed" <<std::endl;
      return 1;
   }
   cout << "Testing BaseContainerArrayMetadata"<<endl;
   if( !testBaseContainerArrayMetadata()) {
      std::cout << "BaseContainerArrayMetadata test failed" <<std::endl;
      return 1;
   }
   cout << "All tests completed successfully!" << endl;
   return 0;
}
