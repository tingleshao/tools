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

   cout << "Starting"<<endl;
   server.Start();

   while(1) {
   }
   cout << "Done"<<endl;

}
