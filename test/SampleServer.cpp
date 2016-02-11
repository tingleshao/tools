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
   server.Start();

   
}
