#include <iostream>
#include <ATimer.h>
#include <AThread.h>
#include <BaseChunkMetadata.h>
#include <BaseContainerMetadata.h>
#include <BaseContainer.h>
#include <BaseChunk.h>
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
   cout << "Testing BaseChunkMetadata"<<endl;
   if( !atl::testBaseChunkMetadata() )
   {
      cout << "BaseChunkMetadata Test Failed!" << endl;
      return 1;
   }
   cout << "Testing BaseChunk"<<endl;
   if( !atl::testBaseChunk() )
   {
      cout << "Test BaseChunk Failed!" << endl;
      return 1;
   }
   cout << "Testing TSArray"<<endl;
   if( !testTSArray()) {
      std::cout << "TSArray test failed" <<std::endl;
      return 1;
   }
   cout << "Testing BaseContainerMetadata"<<endl;
   if( !testBaseContainerMetadata()) {
      std::cout << "BaseContainerMetadata test failed" <<std::endl;
      return 1;
   }
   cout << "Testing BaseContainer"<<endl;
   if( !testBaseContainer()) {
      std::cout << "BaseContainer test failed" <<std::endl;
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


   cout << "All tests completed successfully!" << endl;
   return 0;
}
