/**
 * \file This file creates a terminal application for a specific socket.
 *
 * This application uses the BaseSocket class to connect to a server applications. Any
 * keyboard data entered is passed to a server application. Any data received is 
 * printed to the display
 **/
#include <thread>
#include <iostream>
#include <string>
#include <cstring>

#include <BaseSocket.h>

using namespace std;

//Default settings
const int  defaultPort = 9000;

/**
 *!\brief prints the information about how to test the application
 **/
bool printHelp( void )
{
   cout << "Terminal application\n" << endl;
   cout << "Usage: ./SocketTerminal <options> hostname" << endl;
   cout << "Options:" << endl;
   cout << "\t-h     prints this help menu and exits" << endl;
   cout << "\t-p     specifies the port to listen on (default="<<defaultPort<<")" << endl;
   cout << endl;

   return 1;
}

/**
 * Main function
 **/
int main( int argc, char ** argv )
{
   int16_t port = defaultPort;
   std::string hostname;

   ////////////////////////////////////////////////////////////
   //Parse command line options
   ////////////////////////////////////////////////////////////
   int argCount = 0;
   for( int i = 1; i < argc; i++ ) {
      if( !strcmp(argv[i], "-h" )) {
         printHelp();
         return EXIT_SUCCESS;
      }
      else if( !strcmp(argv[i], "-p" )) {
         argCount++;
         i++;
         if( i > argc ) {
            cout << "-p option must specify a port" << endl;
         }
         port = atoi( argv[i]);
      }
      else if( i == argc-1 ) {
         hostname.assign(argv[i]);
      }
      else {
         cout << "Invalid parameter "<<argv[i]<< endl;
         printHelp();
         return EXIT_SUCCESS;
      }
   }
   if(hostname.empty()) {
      cout << "A hostname to connect to is required. Exiting"<<endl;
      return EXIT_SUCCESS;
   }

   
   ////////////////////////////////////////////////////////////
   // Connect
   ////////////////////////////////////////////////////////////
   atl::BaseSocket client;
   if( !client.createTcpClient( hostname, port )) {
      cerr << "Unable to connect to server at "<<hostname.c_str()<<":"<<port<<endl;
      return EXIT_SUCCESS;
   }

   cout << "Connected to client at "<<hostname.c_str()<<":"<<port<<endl;

   bool running = true;

   string message;
   while( running ) 
   {
      getline( cin, message );

      char msg[message.size()+1];
      copy(message.begin(), message.end(), msg);
      msg[message.size()]  = 0;
      client.sendData( msg, sizeof(msg));

      if( !message.compare("x")) {
         running = false;
      }
   }

   atl::ASleep(1);


   return EXIT_SUCCESS;

}
