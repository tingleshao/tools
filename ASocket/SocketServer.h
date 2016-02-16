//============================================================
/*!\file BaseSocket.h
 * \brief class for making a thread that receives HTTP Jpeg data and produces frames
 * Based on RTSPFrameReceiver.h
 * 
 *
 *<hr>
 * \author Russ Taylor working through ReliaSolve for Aqueti, 2015
 *<hr>
 */
//==============================================================================
#pragma once

//System header files

//Application header files
//#include <Timer.h>
//#include <MemoryManager.h>
#include <vector>
#include <string>
#include <thread>

#include <AThread.h>
#include "BaseSocket.h"

#define SKT_BACKLOG 1000
namespace atl
{
   /**
    *!\brief Class to create a stand-alone server
    **/
   class SocketServer : public BaseSocket, public atl::AThread
   {
      private:
   //      double waitTime = 0.1;                      //!< How long to wait while reading
         bool * running = NULL;                        //!< Flag to signal thread termination
         std::vector<BaseSocketData> socketDataVector; //!<Array of open sockets with data
   
   
         int  addConnection();
         RawDataBuffer readIndex( size_t index );
         void mainLoop( void );
   
      public:
         SocketServer();
         ~SocketServer();
         int  Initialize( int port, int backlog=SKT_BACKLOG );
         bool closeIndex( size_t index );
         int receiveData( double timeout );   
         int bcastData( uint8_t * buffer, size_t bytes );
         int sendData( BaseSocketData data, uint8_t * buffer, size_t bytes );
         int sendData( int index, uint8_t * buffer, size_t bytes );
   
         size_t readSocketData(BaseSocketData * socketData );
   //      virtual int processSocketIndex( int index );
         virtual bool processSocketData( BaseSocketData * data);
   
         
   };
   
   int create_tcp_server(const char *addr, int port, int backlog= SKT_BACKLOG );
   
   
   /** Run tests on the BaseSocket class to make sure it is working
       as expected.  Returns 0 on success and integer related to what failed
       on failure.  Each time a new method or capability is added, tests
       should be added. 
   **/
   bool testSocketServer(void);
}
