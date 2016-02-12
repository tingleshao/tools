#include <iostream>

using namespace std;

#include <SocketServer.h>

/**
 * Main test function
 **/
int main(void)
{
   //Create a socket Server
   SocketServer server;
   server.Initialize( 9000 );
   server.setWaitTime(.1);

   bool running = true;
   server.Start( &running );

   
   std::this_thread::sleep_for(std::chrono::milliseconds(10000));



}
