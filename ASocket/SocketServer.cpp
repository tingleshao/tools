//============================================================
/*!\file SocketServer.cpp
 * \brief Contains the BaseSocket Class
 *
 *<hr>
 * \author Steve Feller based on work by Russell Taylor, May 2015
 *<hr>
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>

#include <cstring>
#include <iostream>
#include <thread>

#include <Timer.h>
#include "SocketServer.h"

using namespace std;
/**
 * Socket server constructor
 **/
SocketServer::SocketServer() 
{
   //Clear the FDset values
   FD_ZERO( &servFds );
   FD_ZERO( &clientFds );
   FD_ZERO( &exceptFds );
}

/**
 *!\brief Destructor
 **/
SocketServer::~SocketServer()
{
   //Close all socket vectors
   for(int i = 0; i < socketDataVector.size(); i++ ) 
   {
      socketDataVector[i].data.deallocate();

      //If we are the same object as our socketData, then we'll clear that 
      //with the base BaseSocket destructor
      closeIndex(i);
   }
}

/**
 * \brief Closes the socket at the specified index
 *
 * \param [in] index specifies which object in the socket vector to close
 * \return true 
 **/
bool SocketServer::closeIndex( size_t index ) 
{
   socketDataVector[index].closeSocket();
   return true;
}

/**
 *!\brief Adds a new connection to the server socket\n"
 **/
int SocketServer::addConnection()
{
   //Create a socketData structure
   BaseSocketData newSocketData = createBaseSocketData( 1024 );

   //Check if we're the server. If so, add a connection
   printf("Got a connection\n");
   sockaddr_in clientname;
   int size = sizeof( clientname );
   
   newSocketData.fd = accept ( socketData.fd,
                          ( struct sockaddr *) &clientname,
                          ( socklen_t *)&size
                          );
    if (newSocketData.fd < 0)
    {
       perror ("error: accept");
       return -1;
    }

    newSocketData.hostname.assign(inet_ntoa(clientname.sin_addr));
    newSocketData.port = clientname.sin_port;

    fprintf (stderr,
             "Server %ld: connect %d from host %s, port %d.\n"
             , socketDataVector.size()
             , newSocketData.fd
             , inet_ntoa (clientname.sin_addr)
             , ntohs (clientname.sin_port));
   
    FD_SET (newSocketData.fd, &servFds);
    if( newSocketData.fd > servFdMax ) { 
       servFdMax = newSocketData.fd;
    }

    //Add this socket data structure to the list
    newSocketData.index = socketDataVector.size();
    socketDataVector.push_back( newSocketData );

   return newSocketData.index;
}


//============================================================
/**
 *!\brief initialization loop for connectiong a server
 *
 * \param [in] port port to connect to
 * \param [in] listenSize number of buffers to keep in the backlog
 *
 * \return 1 on success, negative number of failure
 *
 * This fuction establishes a connection for the server object. 
 **/
int SocketServer::Initialize( int port, int listenSize ) 
{
   //Create and populate the socketData structure
   socketData = createBaseSocketData(1024);

   //Assign the port to the hostname
   socketData.port = port;

   //Create the server socket 
   socketData.fd = create_tcp_server( NULL, socketData.port, listenSize );

   if( socketData.fd < 0 ) {
      fprintf(stderr, "BaseSocket unable to connect to tcp at %s:%d\n"
             , socketData.hostname.c_str()
             , socketData.port );
      return -1;
   }


   //Add this socket data structure to the list
   socketDataVector.push_back( socketData );

   //Add most current socket to the fset
   FD_SET( socketData.fd, &servFds );
   if( servFdMax < socketData.fd ) {
      servFdMax = socketData.fd;
   }

   printf("Server socket: %d, port: %d, index: %lu\n", socketData.fd, socketData.port, socketDataVector.size()-1 );

   return 1;
}

/**
 *!\brief main processing loop
 **/
void SocketServer::mainLoop( void )
{
   receiveData( waitTime );
}

/*
 * \brief handles incoming data
 *
 * \param [in] index data structure with the data
 *
 * For the base case, just print the incoming buffer
 *
bool SocketServer::processSocketIndex( int index ) 
{
   return ( processDataBuffer( socketDataVector[index] ) 
}
*/

/**
 * \brief Processes the data in incoming databuffer object
 * \param [in] sockData BaseSocketData struct with received data
 **/
bool SocketServer::processSocketData( BaseSocketData * sockData)
{
   size_t maxIndex = sockData->data.getMaxIndex();
   if(maxIndex == 0 ) {
      cout << "Socket at index "<<sockData->index<<" receive error:"<<maxIndex<<endl;
      return false;
   }
   else {
      cout << (char *)&sockData->data[0]<<endl;
      received++;
   }

   //Point back to the first element
   sockData->data.setMaxIndex(0);

//   sockData->data.deallocate();

   return true;
}



/**
 *!\brief Function that reads incoming data from the socket object
 *
 * \param [in] refSocketData pointer to a socketData structure
 *
 * \return -1 on error, 1 on success
 **/
size_t SocketServer::readSocketData( BaseSocketData * refSocketData )
{
   int nbytes = 0;
   int ret = 0;

   //Prep reference data
//   if( refSocketData->data.getElementCount() < bufferSize ) {
      refSocketData->data.allocate(bufferSize);
//   }

   size_t elementCount = refSocketData->data.getElementCount();
   size_t maxIndex  = refSocketData->data.getMaxIndex();
   size_t count = elementCount-maxIndex;

   //Read map data at the file descriptor into the refSocketData buffer
   nbytes = read ( refSocketData->fd
                 , (uint8_t *)&refSocketData->data[maxIndex]
                 , count
                 );


   //If no bytes were ready, we have an error
   if (nbytes < 0)
   {
      // Read error. 
      perror ("error: read");
      fprintf(stderr, "Unable to read fd: %d\n", refSocketData->fd );
      return -1;
   }
   else if (nbytes == 0) {
//      cout<<"read "<<nbytes<<" bytes"<<" with maxIndex "<<maxIndex<<endl;
      return 0;
   }

   // Data read. 
   refSocketData->data.setMaxIndex(nbytes+maxIndex);

   //This would be changed here to check if a given number of bytes were received.
   return nbytes+maxIndex;
}


/**
 * \brief Function to check sockets for available data and reads data in
 *
 * \param [in] timeout how long to wait for data to be received
 *
 * \return -1 on failure, 0 on incomplete buffer, 1 on full buffer
 *
 * This function reads data from the socket and fills the SocketData structure
 * with any new data.
 **/
int SocketServer::receiveData( double timeout ) 
{
   Timer timer;
   //Flag to indicate no file descriptors have data
   bool done = false;

   //While we're still not done reading file descriptors...
   do
   {

      //Calculate timeout value, if any.
      double endTime = getTime() + timeout;
      timeval tv = convertDoubleToTimeVal( endTime - getTime());

      //Set the list of socket descriptors to check if data is available
      fd_set readSet = servFds;

      //Select file descriptors with waiting data. If none is available,
      //we are done receiving data.
      int rc = select( servFdMax+1, &readSet, NULL, NULL, &tv );
      if( rc < 0 ) {
         perror( "SocketServer error: select");
         fprintf(stderr, "SocketServer select error: %d\n", rc );
      }
      else if ( rc == 0 ) {
         done = true;
      }
      else 
      {
         //Loop through file descriptors and process pending data
         for( int i = 0; i < socketDataVector.size(); i++ ) 
         {
            if(  socketDataVector[i].fd == -1 ) {
               continue;
            }
            //If the file descriptor is erady to ready, then process it.
            if( FD_ISSET( socketDataVector[i].fd , &readSet ))
            {
               //Check if we're the server. If so, add a connection
               if(socketDataVector[i].fd == socketData.fd) {
                  printf("Adding connection!\n");
                  addConnection(); 
               }
               else
               {
                  //Read the socketDSata 
                  int rc =readSocketData(&socketDataVector[i]);
                  if( rc == 0 ) {
                     //Print message
                     cout << "Socket fd "<< socketDataVector[i].fd
                          << " at index "<<i<<" closed"<<endl;

                     FD_CLR( socketDataVector[i].fd, &servFds );
                     closeIndex(i);
                  }
                  else {
                     if(rc > 0 ) {
                        processSocketData( &socketDataVector[i] );
                     }
                  }
               }
            }
         }
      }
   } while(( !done )&&(timer.elapsed() < timeout ));

   return 1;
}

/**
 * \brief function to broadcast data based on an index value
 * \param [in] buffer data buffer to send
 * \param [in] bytes number of bytes to send
 * \return negative number of failed sends on error, number of successful sends on success
 **/
int SocketServer::bcastData( uint8_t * buffer, size_t bytes )
{
   int fail = 0;
   int success=0;
   //On broadcast (index = -1 ), recursively call function for each value
   for( int i = 1; i < socketDataVector.size(); i++ ) {
      //Begin regular processing
      printf("Writing %ld bytes to %d\n", bytes, socketDataVector[i].fd );
      int rc= sendData( socketDataVector[i], buffer, bytes );
      if( rc < 0 ) {
         fail--;
      }
      else {
         success++;
      }

   }

   if( fail < 0 ) {
      return fail;
   }

   return success;
}

/** 
 * \brief Sends buffer to the socket at the given index
 **/
int SocketServer::sendData( int index, uint8_t * buffer, size_t bytes )
{
   return sendData( socketDataVector[index], buffer, bytes );
}

/**
 * \brief Function to send data to the socket at the specified index
 **/
int SocketServer::sendData( BaseSocketData data, uint8_t * buffer, size_t bytes )
{
   int rc = noint_block_write( data.fd, (uint8_t *)buffer, bytes );
   if( rc < bytes ) {
      printf("Error sending data: %d, %ld\n", rc, bytes);
      return -2;
   }

   return 1;
}


/**
 * \brief funtion to read data from the socket at the given index
 *
 * \param [in] index index of the socket in the array
 **/
RawDataBuffer SocketServer::readIndex( size_t index )
{
   int rc = readSocketData( &socketDataVector[index] );
 
   //Return a valid index 
   RawDataBuffer result = socketDataVector[index].extractData();

   cout << "Result: "<<result.m_bufferSize<<endl;
   return result;
}

/**
 * \brief Establish a TCP connection to the specified host name and port number.
 *
 * \param [in] addr - IP address to connect to
 * \param [in] port - port number ot connect to
 * \param [in] backlog - size of backlog stack 
 *
 * \return -1 on failure and a file descriptor on success.
 **/
int create_tcp_server(const char *addr, int port, int backlog)
{
   struct sockaddr_in sock_in; // The name of the sock_in
   struct hostent *host;       // The host to connect to 
   int sock;

   // set up the socket.  We don't care about the port number
   // or NIC we're using to establish the connection. 
   sock = socket(AF_INET, SOCK_STREAM, 0 );
   if (sock < 0) {
        fprintf(stderr, "open_socket :  "
                        "can't open socket\n");
        return -1;
   }

   sock_in.sin_family      = AF_INET;
   sock_in.sin_port        = htons( port );

   //If not NULL, set address
   if( addr == NULL ) {
      sock_in.sin_addr.s_addr = INADDR_ANY;
   }
   else {
      sock_in.sin_addr.s_addr = inet_addr(addr);
   }

   if (bind(sock, (struct sockaddr *)&sock_in, sizeof(sock_in)) < 0) {
        fprintf(stderr, "bind open_socket: Could not connect to "
                        "machine %d.%d.%d.%d port %d\n",
                (int)((sock_in.sin_addr.s_addr >> 24) & 0xff),
                (int)((sock_in.sin_addr.s_addr >> 16) & 0xff),
                (int)((sock_in.sin_addr.s_addr >> 8) & 0xff),
                (int)((sock_in.sin_addr.s_addr >> 0) & 0xff),
                (int)(ntohs(sock_in.sin_port)));
        close(sock);
        return -1;
   }

   
   if( listen( sock, backlog ) < 0 ) {
      perror("listen");
      return -1;
   }

   
   return sock;
}




/** 
 *!\brief BaseSocket test function
 **/
#define SOCKETCOMMTESTPORT 9000


//============================================================
// TEST Code
// This test code creates a Socket server, then connects with
// recvCount BaseSocket receivers.
//============================================================
bool testSocketServer(void)
{
   bool running = true;
   bool rc = true;

   int recvCount = 10;

   std::string hostname;
   hostname.assign("localhost");
   int port = SOCKETCOMMTESTPORT;
   SocketServer socketServer;

   std::vector<BaseSocket> baseSocketVector;
   baseSocketVector.resize(recvCount);

   int clientIndex = -1;
   int serverIndex = -1;

   //If we're Ok, Create a server object
   int irc = socketServer.Initialize( port, recvCount );
   if( irc < 0 ) {
      printf( "Unable to initialize the socketServer. Exiting\n");
      exit(1);
   }
   socketServer.setWaitTime( 2.0 );

   socketServer.Start();

//   std::thread serverThread = socketServer.Start( &running ); 

   sleep(1);

   //Loop through and send...
   for( int i = 0; i < recvCount; i++ ) 
   {
      //If we're Ok, Create a client object
      if( running ) {
         irc = baseSocketVector[i].Initialize(hostname, port );
         if( irc < 0 ) {
           printf("BaseSocket_test: Unable to Initialize: %d!\n", i);
           running = false;
           rc = false;
         }
      }
   }

   Timer sendTime;
   int messageCount = 0;

   printf("Burst sending messages\n");
   char data[256];

   for( int i = 0; i < 10; i++ ) {
      //Send message
      sprintf( data,"kilroy was here: %d", i);
      printf("Sending: %s\n", data );
      socketServer.bcastData( (uint8_t *) data, strlen(data));
      printf("Sent: %s\n", data );

      char data2[256];
      memset( data2, 0, 256 );

      //Receive message
      for( int i = 0; i < recvCount; i++ ) {
         baseSocketVector[i].recvData(&data2, strlen( data ));
      }
   }
/*
   char data2[256];
   memset( data2, 0, 256 );
   for( int i = 0; i < recvCount; i++ ) {
      baseSocketVector[i].recvData((uint8_t *)&data2, strlen( data ));
   }
*/
/*
   while(sendTime.elapsed() < 2.0 ) {
      for( int i = 0; i < recvCount; i++ ) {
         //Send a text message
         if( running ) {
            char data[256];
            sprintf( data,"kilroy was here: %d", messageCount);
            
            if( baseSocketVector[i].sendData((uint8_t *)data,  strlen(data)) < 0 ) {
               printf("Failed sending data\n");
               rc = false;
               running = false;
            }
            else {
               messageCount++;
            }
           
         }
         usleep(1000);
      }
   }
*/
   double elapsed = sendTime.elapsed();

   running = false;

   socketServer.Stop();
   socketServer.Join();

   printf("Goodbye!\n");

   return rc;
}
