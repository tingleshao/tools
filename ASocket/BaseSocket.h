
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
#ifndef BASESOCKET_H
#define BASESOCKET_H

#include <functional>

//System header files

//Application header files
#include <vector>
#include <string>

#include <ATimer.h>
#include <ExtendedBuffer.tcc>

//Socket status variables
#define SOCK_ERROR        -1            //!<Code shown for the socket if there is an error
#define SOCK_NONE          0            //!<Code for a socket object in undefined state
#define SOCK_CONNECTED     1            //!<Code to show socket is connected

#define DEFAULT_SOCKET_BUFFER_SIZE 1024 //!< Default size of a socket buffer

using namespace std::placeholders;

namespace atl
{
   /**
    * \brief datastructure to manage incoming socket data 
    *
    * The information in this structure is filled as data is read from
    * a socket.
    * - bytes indicates the total number of bytes expected to be * received. 
    * - offset indicates the current offset into the socketData buffer
    **/
   class BaseSocketData {
      public:
         int     fd = -1;                  //!< File descriptor for the socket
         int     port = -1;                //!< port number of the socket
         int     index = 0;                //!< Array index of the socket data
         std::string hostname;             //!< IP Address of the socket
         ExtendedBuffer<uint8_t> data;     //!< Pointer to the data structure for recieving data
   
         ExtendedBuffer<uint8_t> extractData(); 
         int sendData( void * buffer,  size_t bytes );
         void closeSocket();
   }; 
   
   /**
    *!\class BaseSocket 
    *
    * \brief class for handling basic Socket IO. 
    *
    **/
   class BaseSocket 
   {
      public:
         BaseSocket();
         virtual ~BaseSocket();
   
         BaseSocketData socketData;              //!< Structure containing information about a socket
   
         //File descriptors for read, write, and select operations
         fd_set servFds;                       //!< file descriptors for read/write 
         fd_set clientFds;                     //!< file descriptors for receiving data
         fd_set exceptFds;                     //!< file descriptors for exceptions
   
   //      int serverSock  = 0;                  //!< socket of a server (0 => client
   
         int servFdMax   = 0;                  //!< Maximum Server File Descriptor
         int clientFdMax = 0;                  //!< Maximum Client File Descriptor
         int exceptFdMax = 0;                  //!< Maximum exception File Descriptor
   
         int status = 0;                       //!< Variable to indicate status
         uint64_t received = 0;                //!< Number of received packets
         uint64_t sent     = 0;                //!< Number of messages sent
         double waitTime = 0.0;                //!< How long to wait for socket data
   
         size_t bufferSize = 1024;                //!< Size of the data buffer for incoming info
   
         //Implement a callback
         bool setHandleMessageCallback( std::function<void(ExtendedBuffer<uint8_t>)> callback );
         std::function<void(ExtendedBuffer<uint8_t>)> handleMessage;
   
         //Initialization and exit functions
         bool createTcpClient( std::string hostname, int port);
   //      int  createTcpServer( std::string hostname, int port);
   //      bool createTcpListener( int port);
         bool createUdpClient( std::string hostname, int port);
         bool createUdpListener( int port);
   
         //Data transmission functions
         int sendData( void * buffer,  int bytes );
         int recvData( void * buffer,  int bytes );
         int recvDataSize( void * buffer,  int bytes, double timeout );
   
         bool closeIndex( int index );
         void setWaitTime( double wtime );
         void setBufferSize( size_t size );
   
         virtual int  Initialize( std::string hostname, int port);
         virtual bool prepSocketData();
         virtual int  recvSocketData();
         virtual bool processSocketData(BaseSocketData sockData);
         virtual bool processSocketData();
   
   
      private:
         std::string sourceUrl;                //!< Network URL to listen on
   
   };
   
   BaseSocketData createBaseSocketData( size_t size );
   
   ///Opens a socekt at the given address and port
   int open_socket ( const char * IPaddress , unsigned short * portno );
   
   /// Establish a TCP connection to the specified host name and port number.
   // Returns -1 on failure and a file descriptor on success.
   static int connect_tcp_to(const char *addr, int port);
   
   
   /**
    *   This routine will read in a block from the file descriptor.
    * It acts just like the read() routine on normal files, except that
    * it will time out if the read takes too long.
    *   This will also take care of problems caused by interrupted system
    * calls, retrying the read when they occur.
    *   This routine will either read the requested number of bytes and
    * return that or return -1 (in the case of an error) or 0 (in the case
    * of EOF being reached before all the data arrives), or return the number
    * of characters read before timeout (in the case of a timeout).
    **/
   static int noint_block_read_timeout(int infile, unsigned char buffer[], size_t length,
                                       struct timeval *timeout);
   
   /**
    *      This routine will write a block to a file descriptor.  It acts just
    * like the write() system call does on files, but it will keep sending to
    * a socket until an error or all of the data has gone.
    *      This will also take care of problems caused by interrupted system
    * calls, retrying the write when they occur.  It will also work when
    * sending large blocks of data through socket connections, since it will
    * send all of the data before returning.
    * This routine will either write the requested number of bytes and
    * return that or return -1 (in the case of an error) or 0 (in the case
    * of EOF being reached before all the data is sent).
    */
   
   int noint_block_write(int outfile, uint8_t * buffer, size_t length);
   
   /**
    *      This routine will perform like a normal select() call, but it will
    * restart if it quit because of an interrupt.  This makes it more robust
    * in its function, and allows this code to perform properly on pxpl5, which
    * sends USER1 interrupts while rendering an image.
    **/
   static int noint_select(int width, fd_set *readfds, fd_set *writefds,
                           fd_set *exceptfds, struct timeval *timeout);
   
   
   int create_udp_client( const char * addr, int port ); 
   int create_tcp_client( const char * addr, int port ); 
   
   void printMessage(ExtendedBuffer<uint8_t> data );
   bool testBaseSocket(void);
}
#endif
