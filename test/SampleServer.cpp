#include <iostream>

using namespace std;

#include <SocketServer.h>

const int defaultPort = 9000;

/**
 *!\brief prints the information about how to test the application
 **/
bool printHelp( void )
{
   cout << "Terminal application\n" << endl;
   cout << "Usage: ./SocketTerminal port" << endl;
   cout << "Options:" << endl;
   cout << "\t-h     prints this help menu and exits" << endl;
   cout << endl;

   return 1;
}

/**
 * Main test function
 **/
int main(int argc, char ** argv)
{
   int port = defaultPort;              //Port to connect to

   ////////////////////////////////////////////////////////////
   //Parse command line options
   ////////////////////////////////////////////////////////////
   int argCount = 0;
   for( int i = 1; i < argc; i++ ) {
      if( !strcmp(argv[i], "-h" )) {
         printHelp();
         return EXIT_SUCCESS;
      }
      else if( i == argc-1 ) {
         port = atoi( argv[i]);
      }
      else {
         cout << "Invalid parameter "<<argv[i]<< endl;
         printHelp();
         return EXIT_SUCCESS;
      }
   }

   //Create a socket Server
   SocketServer server;
   server.setWaitTime(1);
   server.Initialize( port );


   cout << "Starting SampleServer on port "<<port<<"\n"<<endl;
   server.Start();

   Timer timer;
   while(timer.elapsed() < 60 ) 
   {
      server.receiveData(1);
   }

   cout << "Done"<<endl;

   server.Stop();
   server.Join();
}
