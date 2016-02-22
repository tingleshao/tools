/*!\file BaseSocket.cpp
 * \brief Contains the BaseSocket Class
 *
 *<hr>
 * \author Steve Feller based on work by Russell Taylor, May 2015
 *<hr>
*/
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

#include <ATimer.h>
#include "BaseSocket.h"

namespace atl
{
   /**
    * \brief Extracts the data buffer and releases data internally
    *
    * \return TypeBuffer object with the data
    *
    * This function returns a pointer to the current data buffer and
    * then removes internal references. The calling function is responsible
    * for deleting the data (which is integrated in the DataBuffer class)
    **/
   RawDataBuffer BaseSocketData::extractData() 
   {
      RawDataBuffer rdb;
      rdb.m_buffer = data.m_buffer;
      rdb.m_bufferSize = data.m_bufferSize;
   
      //Clear data
      data.m_buffer = NULL;
      data.m_bufferSize = 0;
   
      return rdb;
   }
   
   
   /**
    * \brief Sends data through the BaseSocketData class
    *
    * \param [in] buffer buffer to send
    * \param [in] bytes number of bytes in buffer
    * \return number of bytes sent on success, negative value on failure
    *
    * This function assumes that the buffer passed is allocated
    * memory with the provided number of bytes. 
    **/
   int BaseSocketData::sendData( void * buffer,  size_t bytes )
   {
      //Make sure we're writing to a valid file descriptor
      if( fd < 0 ) {
         fprintf( stderr, "BaseSocketData asked to send to undefined socket\n");
         return -1;
      }
      int rc = noint_block_write( fd, (uint8_t *)buffer, bytes );
      if( rc < bytes ) {
         printf("Error sending data: %d, %ld\n", rc, bytes);
         return -2;
      }
   
      return rc;
   }
   
   /**
    * \brief Closes the existing connection
    **/
   void BaseSocketData::closeSocket() 
   {
      //Close the socket if it is open
      if( fd > 0 ) {
         close( fd );
         fd = -1;
      }
   }
   
   /**
    *!\brief constructor 
    **/
   BaseSocket::BaseSocket() {
      setHandleMessageCallback( printMessage );
   }
   
   /**
    * \brief destructore
    *
    * Closes any open sockets on exit
    **/
   BaseSocket::~BaseSocket() {
   
      if( socketData.fd != -1 ) {
         printf("Closing socket %d. Sent %ld and received %ld messages\n"
               , socketData.fd
               , sent
               , received 
               );
         close( socketData.fd );
         socketData.fd = -1;
      }
   }
   
   /**
    * \brief Adds a callback handler
    * 
    * \param [in] callback pointer to the callback function
    **/
   bool BaseSocket::setHandleMessageCallback( std::function<void(TypeBuffer<uint8_t>)> callback )
   {
      handleMessage = std::bind(callback, _1 );
      return true;
   }
   
   
   
   /**
    * \brief function that processes data received from a socket
    *
    * For the base case, just print the incoming buffer size
    **/
   bool BaseSocket::processSocketData() 
   {
      try {
         handleMessage( socketData.data.getTypeBuffer());
      } catch(std::bad_function_call& e) 
      {
         cerr << "handleMessage callback not set!" << endl;
         /*
         //See if we have received any elements
         if( socketData.data.m_bufferSize > 0 )
         {
            printf( "Client with fd %d received: \"%s\"\n"
               , socketData.fd
               , (char *)&socketData.data[0]
               );
   
            received++;
            return true;
         }
   
   
         return false;
         */
      }
   
      return true;
   }
   
   /**
    * \brief function that processes data received from a socket
    *
    * For the base case, just print the incoming buffer size
    **/
   bool BaseSocket::processSocketData( BaseSocketData sockData ) 
   {
      if( sockData.data.getElementCount() > 0 ) {
         printf( "Client with fd %d received: \"%s\"\n"
            , sockData.fd
            , (char *)&sockData.data[0]
            );
   
         received++;
         return true;
      }
      else {
         return false;
      }
   }
   
   
   
   /**
    * \brief sets the size of the buffer used for acquiring data
    *
    * \param [in] size the size of the acquisition buffer
    **/
   void BaseSocket::setBufferSize( size_t size ) {
      bufferSize = size;
   
      return;
   }
   
   /**
    *
   int open_socket( const char * addr, int port ) 
   {
      struct sockaddr_in sock_in; // The name of the sock_in 
      struct hostent *host;      // The host to connect to 
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
   
      return sock;
   }
   */
    
   /**
    * \brief creates a client socket
    *
    * \param [in] addr name of the host to connect to
    * \param [in] port port number to connect to
    *
    * \return negative value on failure, file descriptor on success
    *
    * This function connects to the given UDP server.
    **/
   int create_udp_client( const char * addr, int port ) 
   {
      int udp_socket;
      struct sockaddr_in udp_name;
      struct hostent * remoteHost;
      int udp_namelen;
   
   
      udp_socket = socket(AF_INET, SOCK_DGRAM, 0 );
      if (udp_socket == -1) {
         fprintf(stderr,"create_udp_client(): Could not open socket\n");
         return -1;
      }
   
      udp_namelen = sizeof(udp_name);
   
      memset((void *) &udp_name, 0, udp_namelen);
      udp_name.sin_family = AF_INET;
   
   // gethostbyname() fails on SOME Windows NT boxes, but not all,
   // if given an IP octet string rather than a true name. 
   // MS Documentation says it will always fail and inet_addr should 
   // be called first. Avoids a 30+ second wait for
   // gethostbyname() to fail.
      if ((udp_name.sin_addr.s_addr = inet_addr(addr)) == INADDR_NONE) {
         remoteHost = gethostbyname(addr);
         if (remoteHost) {
             memcpy(&(udp_name.sin_addr.s_addr), remoteHost->h_addr,  remoteHost->h_length);
         }
         else {
            close(udp_socket);
            fprintf( stderr,
                     "UDPFrameReceiver_connect_udp_port: error finding host by name (%s).\n"
                    , addr 
                    );
            return -1;
         }
     }
     udp_name.sin_port = htons(port);
   
     if ( connect(udp_socket,(struct sockaddr *) &udp_name,udp_namelen) ) {
         fprintf(stderr,"UDPFrameReceiver_connect_udp_port: can't bind udp socket.\n");
         
         close(udp_socket);
         return -1;
     }
   
     // Find out which port was actually bound
     udp_namelen = sizeof(udp_name);
     if (getsockname(udp_socket, (struct sockaddr *) &udp_name,
                     (socklen_t *) &udp_namelen)) {
       fprintf(stderr, "UDPFrameReceiver_connect_udp_port: cannot get socket name.\n");
       close(udp_socket);
       return -1;
     }
   
     // NOTE NIC will be 0.0.0.0 if we listen on all NICs. 
   
     // Set up buffering as needed for high-speed sending of multiple frames on
     // the outbound socket.  We want to leave enough room to handle it even if
     // we send a number of individual packets in a burst.  We set the buffer
     // size to a little over four full image frames; internally,
     // the OS reserves twice that many to support overhead.
     // In practice, the packets are usually much smaller than
     // this, so this should be conservative.
     // We're not going to be able to keep up with the packets if we can't set this,
     // so we tell the user how to make it possible for us to do it.
   /*  
     int sendSize = 4 * BIN2_IMG_WIDTH * BIN2_IMG_HEIGHT_ADJUSTED + 256;
     if (0 != setsockopt(udp_socket, SOL_SOCKET, SO_SNDBUF, &sendSize, sizeof(sendSize))) {
      perror("UDPFrameReceiver_connect_udp_port: WARNING: Could not set send buffer size.");
     }
     int actualSendBufSize;
     unsigned int readSize = static_cast<unsigned int>(sizeof(actualSendBufSize));
     getsockopt(udp_socket, SOL_SOCKET, SO_SNDBUF, &actualSendBufSize, &readSize);
     if (actualSendBufSize < sendSize) {
      fprintf(stderr, "UDPFrameReceiver_connect_udp_port: Could not set desired send buffer size\n");
      fprintf(stderr,"Trying to set it to %d, got %d\n", sendSize, actualSendBufSize);
      fprintf(stderr, "Try: sudo sysctl -w net.core.wmem_max=40000000\n");
      close(udp_socket);
      return -1;
     }
   */
     return udp_socket;
   }
   
   /**
    * \brief creates a client socket
    *
    * \param [in] addr name of the host to connect to
    * \param [in] port port number to connect to
    *
    * \return negative value on failure, file descriptor on success
    *
    * This function connects to the given UDP server.
    **/
   int create_tcp_client( const char *addr, int port ) 
   {
      int tcpSocket;
      struct sockaddr_in client;              // The name of the sock_in 
      
      struct hostent * phe;                   // pointer to host information entry 
      phe = gethostbyname(addr);
   
   
      // set up the socket.  We don't care about the port number
      // or NIC we're using to establish the connection. 
      tcpSocket = socket(AF_INET, SOCK_STREAM, 0 );
      if (tcpSocket < 0) {
           fprintf(stderr, "BaseSocket::create_tcp_client:  "
                           "can't open socket\n");
           return -1;
      }
   
       client.sin_addr.s_addr = inet_addr(phe->h_addr);
       client.sin_family = AF_INET;
       client.sin_port = htons( port );
       if ( phe != NULL) {
          memcpy((void *) &client.sin_addr, (const void *) phe->h_addr, phe->h_length);
       } else {
          fprintf(stderr, "create_tcp_client: port %d: can't get %s host entry\n", port, addr);
          return -1;
       }
   
       //Connect to remote client
       if (connect(tcpSocket, (struct sockaddr *)&client , sizeof(client)) < 0)
       {
           perror("connect failed. Error");
           fprintf(stderr, "create_tcp_client: Could not connect to "
                           "machine %d.%d.%d.%d port %d\n",
                   (int)((client.sin_addr.s_addr >> 24) & 0xff),
                   (int)((client.sin_addr.s_addr >> 16) & 0xff),
                   (int)((client.sin_addr.s_addr >> 8) & 0xff),
                   (int)((client.sin_addr.s_addr >> 0) & 0xff),
                   (int)(ntohs(client.sin_port)));
           return -1;
       }
   
       return tcpSocket;
   }
   
   /**
    * \brief replaces a select call to restart if it received an interrupt
    *
    * \param [in] width highest numbered file descriptor to use
    * \param [in] readfds file descriptors to be read
    * \param [in] writefds file descriptors to see if a write is blocked
    * \param [in] exceptfds file descriptors to watch for exceptions
    * \param [in] timeout amount of time to wait before timing out.
    *
    * \return -1 on failure, number of file descriptors on success
    *
    * This routine will perform like a normal select() call, but it will
    * restart if it quit because of an interrupt.  This makes it more robust
    * in its function, and allows this code to perform properly on pxpl5, which
    * sends USER1 interrupts while rendering an image.
    **/
   
   //int BaseSocket::noint_select(int width, fd_set *readfds, fd_set *writefds,
   int noint_select(int width, fd_set *readfds, fd_set *writefds,
                         fd_set *exceptfds, struct timeval *timeout)
   {
       fd_set tmpread, tmpwrite, tmpexcept;
       int ret;
       int done = 0;
       struct timeval timeout2;
       struct timeval *timeout2ptr;
       struct timeval start, stop, now;
   
       // If the timeout parameter is non-NULL and non-zero, then we
       // may have to adjust it due to an interrupt.  In these cases,
       // we will copy the timeout to timeout2, which will be used
       // to keep track.  Also, the stop time is calculated so that
       // we can know when it is time to bail. 
       if ((timeout != NULL) &&
           ((timeout->tv_sec != 0) || (timeout->tv_usec != 0))) {
           timeout2 = *timeout;
           timeout2ptr = &timeout2;
           gettimeofday(&start, NULL);         // Find start time 
           stop = TimevalSum(start, *timeout); // Find stop time 
       }
       else {
           timeout2ptr = timeout;
           stop.tv_sec = 0;
           stop.tv_usec = 0;
       }
   
       // Repeat selects until it returns for a reason other than interrupt 
       do {
           // Set the temp file descriptor sets to match parameters each time
           // through.
           if (readfds != NULL) {
               tmpread = *readfds;
           }
           else {
               FD_ZERO(&tmpread);
           }
           if (writefds != NULL) {
               tmpwrite = *writefds;
           }
           else {
               FD_ZERO(&tmpwrite);
           }
           if (exceptfds != NULL) {
               tmpexcept = *exceptfds;
           }
           else {
               FD_ZERO(&tmpexcept);
           }
   
           // Do the select on the temporary sets of descriptors 
           ret = select(width, &tmpwrite, NULL, NULL, timeout2ptr);
           if (ret >= 0) { // We are done if timeout or found some 
               done = 1;
           } else if (errno != EINTR) { // Done if non-intr error 
               done = 1;
           }
           else if ((timeout != NULL) &&
                    ((timeout->tv_sec != 0) || (timeout->tv_usec != 0))) {
   
               // Interrupt happened.  Find new time timeout value 
               gettimeofday(&now, NULL);
               if (TimevalGreater(now, stop)) { // Past stop time 
                   done = 1;
               }
               else { // Still time to go.
                   unsigned long usec_left;
                   usec_left = (stop.tv_sec - now.tv_sec) * 1000000L;
                   usec_left += stop.tv_usec - now.tv_usec;
                   timeout2.tv_sec = usec_left / 1000000L;
                   timeout2.tv_usec = usec_left % 1000000L;
               }
           }
       } while (!done);
       // Copy the temporary sets back to the parameter sets 
       if (readfds != NULL) {
           *readfds = tmpread;
       }
       if (writefds != NULL) {
           *writefds = tmpwrite;
       }
       if (exceptfds != NULL) {
           *exceptfds = tmpexcept;
       }
   
       return (ret);
   }
   
   /**
    * \brief This routine will read in a block from the file descriptor.
    *
    * It acts just like the read() routine on normal files, except that
    * it will time out if the read takes too long.
    *   This will also take care of problems caused by interrupted system
    * calls, retrying the read when they occur.
    *   This routine will either read the requested number of bytes and
    * return that or return -1 (in the case of an error) or 0 (in the case
    * of EOF being reached before all the data arrives), or return the number
    * of characters read before timeout (in the case of a timeout).
    **/
   
   int noint_block_read_timeout( int infile
                               , unsigned char buffer[]
                               , size_t length
                               , struct timeval *timeout)
   {
       size_t sofar;     /* How many we read so far */
       int ret; /* Return value from the read() */
       struct timeval timeout2;
       struct timeval *timeout2ptr;
       struct timeval start, stop, now;
   
       // TCH 4 Jan 2000 - hackish - Cygwin will block forever on a 0-length
       // read(), and from the man pages this is close enough to in-spec that
       // other OS may do the same thing.
   
       if (!length) {
           return 0;
       }
   
       /* If the timeout parameter is non-NULL and non-zero, then we
        * may have to adjust it due to an interrupt.  In these cases,
        * we will copy the timeout to timeout2, which will be used
        * to keep track.  Also, the current time is found so that we
        * can track elapsed time. */
       if ((timeout != NULL) &&
           ((timeout->tv_sec != 0) || (timeout->tv_usec != 0))) {
           timeout2 = *timeout;
           timeout2ptr = &timeout2;
           gettimeofday(&start, NULL);         /* Find start time */
           stop = TimevalSum(start, *timeout); /* Find stop time */
       }
       else {
           timeout2ptr = timeout;
       }
   
       sofar = 0;
       do {
           int sel_ret;
           fd_set readfds, exceptfds;
   
           /* See if there is a character ready for read */
           FD_ZERO(&readfds);
           FD_SET(infile, &readfds);
           FD_ZERO(&exceptfds);
           FD_SET(infile, &exceptfds);
           
           sel_ret = noint_select(static_cast<int>(infile) + 1, &readfds,
                                       NULL, &exceptfds, timeout2ptr);
           if (sel_ret == -1) { /* Some sort of error on select() */
               return -1;
           }
           if (FD_ISSET(infile, &exceptfds)) { /* Exception */
               return -1;
           }
           if (!FD_ISSET(infile, &readfds)) { /* No characters */
               if ((timeout != NULL) && (timeout->tv_sec == 0) &&
                   (timeout->tv_usec == 0)) {      /* Quick poll */
                   return static_cast<int>(sofar); /* Timeout! */
               }
           }
   
           /* See what time it is now and how long we have to go */
           if (timeout2ptr) {
               gettimeofday(&now, NULL);
               if (TimevalGreater(now, stop)) { /* Timeout! */
                   return static_cast<int>(sofar);
               }
               else {
                   timeout2 = TimevalDiff(stop, now);
               }
           }
   
           if (!FD_ISSET(infile, &readfds)) { /* No chars yet */
               ret = 0;
               continue;
           }
   
           {
               int nread;
               nread = recv(infile, buffer + sofar,
                            static_cast<int>(length - sofar), 0);
               sofar += nread;
               ret = nread;
           }
   
       } while ((ret > 0) && (sofar < length));
       if (ret == -1) return (-1); /* Error during read */
       if (ret == 0) return (0); /* EOF reached */
   
   
       return static_cast<int>(sofar); /* All bytes read */
   }
   
   /** 
    *!\brief fucntion to write to a file descriptor
    **/
   int noint_block_write(int outfile, uint8_t * buffer, size_t length)
   {
       int sofar = 0; /* How many characters sent so far */
       int ret;   /* Return value from write() */
   
       do {
           /* Try to write the remaining data */
           ret = write(outfile, buffer + sofar, length - sofar);
           sofar += ret;
   
           /* Ignore interrupted system calls - retry */
           if (ret == -1) {
               ret = 1;    /* So we go around the loop again */
               sofar += 1; /* Restoring it from above -1 */
           }
   
       } while ((ret > 0) && (static_cast<size_t>(sofar) < length));
   
       if (ret == -1) return (-1); /* Error during write */
       if (ret == 0) return (0);   /* EOF reached */
   
       return (sofar); /* All bytes written */
   }
   
   /**
    * \brief creates a UDP listener
    *
    * \param [in] port port number to connect to
    *
    * \return negative value on failure, file descriptor on success
    *
    * This function connects to the given UDP server.
    **/
   int create_udp_listener( int port ) 
   {
      int udp_socket;
      struct sockaddr_in servaddr;
      socklen_t len;
   
      udp_socket = socket(AF_INET, SOCK_DGRAM, 0 );
      if (udp_socket == -1) {
         fprintf(stderr,"create_udp_client(): Could not open socket\n");
         return -1;
      }
   
      bzero(&servaddr,sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
      servaddr.sin_port=htons(port);
      if( bind(udp_socket,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 ) {
         perror("Unable to bind");
      }   
   
     return udp_socket;
   }
   
   /*
    *!\brief function that creates a TcpServer
    *
    *\return server index on success -1 on error
    **
   int BaseSocket::createTcpServer( int port, const char * hostname )
   {
      //Create and populate the socketData structure
      BaseSocketData socketData = createBaseSocketData(0);
      int serverSock;
   
      //If we have a valid hostname, assign it to the hostname
      if( hostname != NULL ) {
         socketData.hostname.assign( hostname );
      }
   
      //Assign the port to the hostname
      socketData.port = port;
   
      if( socketData.hostname.empty()) {
         serverSock = create_tcp_server( NULL, socketData.port);
         socketData.fd = serverSock;
      }
      else {
         serverSock = create_tcp_server( socketData.hostname.c_str(), socketData.port);
         socketData.fd = serverSock;
      }
   
      if( socketData.fd < 0 ) {
         fprintf(stderr, "BaseSocket unable to connect to tcp at %s:%d\n"
                , socketData.hostname.c_str()
                , socketData.port );
         return -1;
      }
   
      //Add most current socket to the fset
      FD_ZERO( &servFds);
      FD_SET( socketData.fd, &servFds );
   
      //Add this socket data structure to the list
      socketVector.push_back( socketData );
   
   
      while( running ) 
      {
         fd_set readSet = servFds;
   
         if( select( servFdMax, &readSet, NULL, NULL, NULL ) < 0 )
         {
            perror( "select");
            return -2;
         }
   
         //Loop through all with input
         for( int i = 0; i < servFdMax; i++ ) 
         {
            if( FD_ISSET( i, &readSet ))
            {
               socketData = createBaseSocketData( socketDataBufferSize );
   
               //Check if we're the server
               if( i == serverSock ) {
                  sockaddr_in clientname;
                  int size = sizeof( clientname );
                  
                  socketData.fd = accept (serverSock,
                                         (struct sockaddr *) &clientname,
                                         (socklen_t *)&size
                                         );
                   if (socketData.fd < 0)
                   {
                      perror ("accept");
                      exit (EXIT_FAILURE);
                   }
   
                   socketData.hostname.assign(inet_ntoa(clientname.sin_addr));
                   socketData.port = clientname.sin_port;
   
                   fprintf (stderr,
                            "Server: connect from host %s, port %hd.\n",
                            inet_ntoa (clientname.sin_addr),
                            ntohs (clientname.sin_port));
   
                  
                   FD_SET (socketData.fd, &servFds);
               }
            }
            else
            {
               if( read_from_client(&socketVector[i]) < 0 ) {
                  close(i);
                  FD_CLR( i, &servFds );
               }
            }
         }
      }
   
      return (int)socketVector.size() + 1;
   }
   */
   
   /**
    *!\brief Initialization function
    **/
   int BaseSocket::Initialize( std::string hostname, int port )
   {
      if( !createTcpClient( hostname, port )) {
         return -1;
      }
   
      return 1;
   }
   
   /**
    *!\brief function that creates a TcpClient
    *
    * \param [in] hostname name of the system to connect
    * \param [in] port port to connect to on the remote machine
    *
    * \return array index on success, negative number on failure
    *
    * This function creates a scoketData objects and adds it to the socketVector list.
    * The BaseBaseSocketData structure contains
    **/
   bool BaseSocket::createTcpClient( std::string hostname, int port )
   {
   //   BaseSocketData socketData;
      socketData.hostname.assign( hostname.c_str());
      socketData.port = port;
      printf("Hostname: %s:%d\n", socketData.hostname.c_str(), socketData.port );
      socketData.fd = create_tcp_client( socketData.hostname.c_str(), socketData.port );
      if( socketData.fd < 0 ) {
         fprintf( stderr, "BaseSocket unable to connect to tcp at %s:%d\n"
                , socketData.hostname.c_str()
                , socketData.port 
                );
         status = SOCK_ERROR;
         return false;
      }
   
      FD_SET( socketData.fd, &servFds );
      if( clientFdMax < socketData.fd ) {
         clientFdMax = socketData.fd;
      }
   
      printf("Base socket returning fd of %d\n", socketData.fd );
   
      //Return the index into the list
      return true;
   }
   
   /**
    *!\brief function that creates a TcpClient
    *
    * \param [in] hostname name of the system to connect
    * \param [in] port port to connect to on the remote machine
    *
    * \return array index on success, negative number on failure
    *
    * This function creates a scoketData objects and adds it to the socketVector list.
    * The BaseBaseSocketData structure contains
    **/
   bool BaseSocket::createUdpClient( std::string hostname, int port )
   {
   //   BaseSocketData socketData;
      socketData.hostname.assign( hostname.c_str());
      socketData.port = port;
      printf("Hostname: %s:%d\n", socketData.hostname.c_str(), socketData.port );
      socketData.fd = create_udp_client( socketData.hostname.c_str(), socketData.port );
      if( socketData.fd < 0 ) {
         fprintf( stderr, "BaseSocket unable to connect to tcp at %s:%d\n"
                , socketData.hostname.c_str()
                , socketData.port 
                );
         status = SOCK_ERROR;
         return false;
      }
   
      FD_SET( socketData.fd, &servFds );
      if( clientFdMax < socketData.fd ) {
         clientFdMax = socketData.fd;
      }
   
      //Return the index into the list
      return true;
   }
   
   /**
    *!\brief function that creates a TcpClient
    *
    * \param [in] port port to connect to on the remote machine
    * \return true on success, false on failure
    *
    * This function creates a scoketData objects and adds it to the socketVector list.
    * The BaseBaseSocketData structure contains
    **/
   bool BaseSocket::createUdpListener( int port )
   {
   //   BaseSocketData socketData;
      socketData.port = port;
      socketData.fd = create_udp_listener( socketData.port );
      if( socketData.fd < 0  ) {
         fprintf( stderr, "BaseSocket unable to connect to tcp at %s:%d\n"
                , socketData.hostname.c_str()
                , socketData.port 
                );
         return false;
      }
   
      FD_SET( socketData.fd, &servFds );
      if( clientFdMax < socketData.fd ) {
         clientFdMax = socketData.fd;
      }
   
      //Return the index into the list
      return true;
   }
   
   
   
   
   
   
   /** 
    * \brief sets how log to wait for incoming data
    * 
    * \param [in] wTime wait time as a double
    **/
   void BaseSocket::setWaitTime( double wTime ) {
      waitTime = wTime;
   }
   
   /**
    * \brief Function that sends data over the connection specified by index
    *
    * \param [in] buffer pointer to the data to send
    * \param [in] bytes number of bytes to send
    * \return negative number on failure, number of bytes written on success
    **/
   //int BaseSocket::sendData( int index, uint8_t * buffer, size_t size ) 
   int BaseSocket::sendData( void * buffer, int bytes )
   {
      int rc = socketData.sendData( buffer, bytes);
   
      if( rc > 0 ) {
         sent ++;
      }
   
      return rc;
   }
   
   /**
    * \brief Function that fills the given buffer data from the socket
    *
    * \param [in] buffer data buffer to be filled
    * \param [in] count  number of bytes to read
    *
    * This function receives at most count bytes and writes them into the provided
    * buffer.
    **/
   int BaseSocket::recvData( void * buffer, int count )
   {
      //Begin regular processing
      int rc = 0;
   
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 0;
   
   /* 
      rc = noint_block_read_timeout( socketData.fd, buffer, count, &tv );
   */
   
   //SDF nont blocking
      fd_set input;
      FD_ZERO( &input );
      FD_SET( socketData.fd, &input );
   
      int n = select( socketData.fd+1, &input, NULL, NULL, &tv );
      if( n == -1 ) {
         fprintf( stderr, "BaseSocket Unable to select\n");
         return -1;
      }
      else if ( n == 0 ) {
         return 0;
      }
   
      if( !FD_ISSET( socketData.fd, &input )) {
         fprintf( stderr, "BaseSocket Unable to select( not set )\n");
         return -1;
      }
      //SDF rc = read( socketData.fd, buffer, count );
      cout << "receiving "<<count<<" bytes from fd "<<socketData.fd<<endl;
      rc = recv( socketData.fd, buffer, count, 0 );
      cout << "received "<<count<<" bytes from fd "<<socketData.fd<< " with code"<<rc<<endl;
   
   
   /*
      ioctl(socketData.fd, FIONREAD, &rc );
   //   if( rc ==  0 ) {
         rc = read( socketData.fd, buffer, count );
         if( rc < 0 ) {
            printf("Error reading data: %d, %d from %d\n", rc, count, socketData.fd  );
            perror("recvData");
            return -2;
         }
      }
   */
      //If we got any data, increment hte count of how many we have
      if( rc > 0 ) {
         printf("Received: %s\n", (char *)buffer );
         received++;
      }
   
      return rc;
   }
   
   /**
    *!\brief This function loops until the specified number of bytes are received.
    *
    * This function will wait at most timeout seconds
    **/
   int BaseSocket::recvDataSize( void * buffer, int count, double timeout )
   {
      int nbytes = 0;
      Timer timer;
      //Wait until timeout (if it is not 0 )
      while(( nbytes < count )&&((timer.elapsed() < timeout )||(timeout == 0.0 )))  {
         int rc = recvData( (char *)buffer + nbytes, count - nbytes );
         if( rc < 0 ){
            printf("recvDataSize Error\n");
            return -1;
         }
   
         nbytes+= rc;
      }
   
      if( count == nbytes ) {
         printf("received %d bytes\n", nbytes );
         return nbytes;
      }
   
      return 0;
   }
   
   
   /**
    * \brief prepare the socketData structure to receive incoming data
    *
    * \return 1 on success 
    *
    * This function checks if the socketData structure needs to be reset (first
    * instance or previous instance complete). If so, it reallocates it's data;
    **/
   bool BaseSocket::prepSocketData() 
   { 
      int rc = 0;                          //!< return code
   
      cout <<"SDF prepSocketData for size "<<bufferSize<<endl;
      //Check the BaseData is defined
      if( socketData.data.m_buffer == NULL ) {
         socketData.data.allocate(bufferSize);
      }
   
      //reallocate buffer if necessary. Otherwise
      if(socketData.data.m_bufferSize < bufferSize ) {
        socketData.data.allocate(bufferSize, true );
      }
   
      //0 indicates no change
      return true;
   }
   
   
   /**
    *!\brief fill the socketData structure with incoming information.
    **/
   int BaseSocket::recvSocketData() 
   {
      //Read in the data
      int rc = recvData(socketData.data.m_buffer.get(), socketData.data.m_bufferSize - socketData.data.getMaxIndex());
   
      //If we read valid bytes, increment the offset
      if( rc > 0 ) {
         socketData.data.setMaxIndex( socketData.data.getMaxIndex()+rc);
      }
   
      //Return number of bytes read
      return rc;
   }
   
   /**
    *!\brief Closes the file descriptor at the given index
    **/
   bool BaseSocket::closeIndex( int index )
   {
      printf("Closing index: %d, fd: %d\n", index, socketData.fd );
      FD_CLR( socketData.fd, &servFds ); 
      socketData.closeSocket();
   
      return true;
   }
   
   /**
    *!\brief creates an empty socket data structure with a buffer of the specified size
    **/
   BaseSocketData createBaseSocketData( size_t dataSize)
   {
      BaseSocketData socketData;
   
   
      return socketData;
   }
   
   
   
   #define SOCKETCOMMTESTPORT 9001            //!< Default Communication port for testing
   
   
   //************************************************************
   // TEST Code
   //************************************************************
   /**
    * \brief Callback for received data
    **/
   void printMessage( TypeBuffer<uint8_t> data ) {
      cout << "Default:"<< (char *)&data.m_buffer[0] << endl;
   }
   
   /** 
    *!\brief BaseSocket test function
    *
    * Creates a number of clients an receives a message from each
    **/
   bool testBaseSocket(void)
   {
      BaseSocket udpClient;
      BaseSocket udpListener;
      std::string name;
      name.assign("localhost");
      int port = 3883;
   
      if( udpListener.createUdpListener( port )) {
         printf("Listener listening on port: %d\n", port );
      }
      else {
         fprintf(stderr, "testBaseSocket Unable to establish listener on port %d\n", port);
         return false;
      }
   
      sleep(1);
   
      if( udpClient.createUdpClient( name, port )) {
         printf("client bound to port: %s:%d\n", name.c_str(), port );
      }
      else {
         fprintf(stderr, "testBaseSocket Unable to establish client\n");
         return false;
      }
   
      //Write some data
      char data[256] = "Here is a message";
      char data2[256];
      memset( data2, 0, 256 );
   
      udpClient.sendData( &data, strlen(data)); 
   
      udpListener.recvData( (uint8_t *)&data2,  strlen(data));
   
      printf("%s => %s\n", data, data2 );
   
      return true;
   }
}
